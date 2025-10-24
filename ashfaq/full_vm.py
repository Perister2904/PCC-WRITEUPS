#!/usr/bin/env python3
"""
AshfaqVM emulator implementing the instruction set from ashfaq-vm.c (FUN_00102199 and helpers).

Goal: faithfully execute chal.ashfaq, allow providing input, and instrument comparisons to recover the flag.
"""
from __future__ import annotations
from dataclasses import dataclass
from typing import Optional, Tuple, List


def u32(x: int) -> int:
    return x & 0xFFFFFFFF


@dataclass
class CompareEvent:
    lhs: int
    rhs: int
    z: bool
    lt: bool
    last_input_index: Optional[int]


class AshfaqVM:
    def __init__(self, program: bytes):
        # Program bytes (stream of 16-bit opcodes with immediates)
        self.prog = program

        # Registers R1.. R? (use 16 for safety). C code uses param_1[0..7] for R1..R8
        self.regs: List[int] = [0] * 16

        # Special registers/fields (mirror offsets in C layout)
        # We'll keep pc, sp, bp as explicit vars and also mirror into regs_offsets when natives read them
        self.pc: int = 0  # program counter in bytes into self.prog (C uses words index; but decomp adds bytes).
        self.sp: int = 0x1000
        self.bp: int = 0x1000

        # Flags
        self.flag_Z: bool = False  # at +0x2334
        self.flag_X: bool = False  # at +0x2335 (always set to 0 on cmp in C)
        self.flag_LT: bool = False  # at +0x2336
        self.cmp_result: int = 0

        # Memory regions
        self.stack_mem = bytearray(0x1000)  # stack region (offset 0x102c in C)
        self.stored = bytearray(0x1000)     # stored strings region (offset 0x202c in C)
        self.stored_size: int = 0           # used bytes in stored (at +0x232c)

        # Input buffer region (offset 0x222c in C)
        self.input_buf = bytearray(0x400)
        self.input_len: int = 0  # at +0x232b

        # I/O capture
        self.stdout: bytearray = bytearray()

        # Control
        self.error_flag: bool = False  # corresponds to param_1[7] == 0xeeeeffff
        self.halted: bool = False

        # Instrumentation
        self.last_input_index: Optional[int] = None
        self.last_compare: Optional[CompareEvent] = None
        self.compare_log: List[CompareEvent] = []

    # Helpers to mirror special fields into the param_1-style layout when natives expect them
    def _get_param(self, index: int) -> int:
        # index is in 32-bit words: param_1[index]
        if index == 0:
            return self.regs[0]
        if index == 1:
            return self.regs[1]
        if index == 2:
            return self.regs[2]
        if index == 3:
            return self.regs[3]
        if index == 7:
            return 0xEEEEFFFF if self.error_flag else 0
        if index == 8:
            return self.pc  # also used as length low byte in native print (only low byte matters)
        if index == 9:
            return self.sp
        if index == 10:
            return self.bp
        if index == 0x8cc:
            return self.cmp_result
        # Fallback to regs for others
        return self.regs[index] if 0 <= index < len(self.regs) else 0

    def _set_param(self, index: int, value: int) -> None:
        value = u32(value)
        if index == 0:
            self.regs[0] = value
            return
        if index == 1:
            self.regs[1] = value
            return
        if index == 2:
            self.regs[2] = value
            return
        if index == 3:
            self.regs[3] = value
            return
        if index == 7:
            self.error_flag = (value == 0xEEEEFFFF)
            return
        if index == 8:
            self.pc = value & 0xFFFF  # PC kept 16-bit in C loop; safe cap
            return
        if index == 9:
            self.sp = value & 0xFFFF
            return
        if index == 10:
            self.bp = value & 0xFFFF
            return
        if index == 0x8cc:
            self.cmp_result = value
            return
        if 0 <= index < len(self.regs):
            self.regs[index] = value

    # Memory helpers
    def _stack_write_u32(self, offset: int, val: int) -> None:
        self.stack_mem[offset:offset+4] = u32(val).to_bytes(4, 'little')

    def _stack_read_u32(self, offset: int) -> int:
        return int.from_bytes(self.stack_mem[offset:offset+4], 'little')

    def _store_add_string(self, uid: int, length: int, copy_from_prog: bool) -> None:
        # Write struct: [uid:4][len:1][data:len]
        base = self.stored_size
        if base + 5 + length > len(self.stored):
            self.error_flag = True
            return
        self.stored[base:base+4] = u32(uid).to_bytes(4, 'little')
        self.stored[base+4] = length & 0xFF
        if copy_from_prog:
            for i in range(length):
                if self.pc >= len(self.prog):
                    self.error_flag = True
                    break
                self.stored[base+5+i] = self.prog[self.pc]
                self.pc += 1
        else:
            self.stored[base+5:base+5+length] = self.input_buf[:length]
        # match C: stored_size += length + 6
        self.stored_size = base + 6 + length

    def _store_find_uid(self, uid: int) -> int:
        # Return offset to length byte (base+4) or 0xEEEEFFFF sentinel
        off = 0
        while off < self.stored_size:
            if int.from_bytes(self.stored[off:off+4], 'little') == u32(uid):
                return off + 4
            length = self.stored[off+4]
            if length == 0:
                return 0xEEEEFFFF
            off += length + 6
        return 0xEEEEFFFF

    def _store_load_to_input(self, uid: int) -> None:
        off = self._store_find_uid(uid)
        if off == 0xEEEEFFFF:
            self.error_flag = True
            return
        length = self.stored[off]
        self.input_len = length
        self.input_buf[:length] = self.stored[off+1:off+1+length]

    # Read helpers from program stream
    def _read_u16(self) -> int:
        if self.pc + 2 > len(self.prog):
            return 0
        val = int.from_bytes(self.prog[self.pc:self.pc+2], 'little')
        self.pc += 2
        return val

    def _read_u32(self) -> int:
        if self.pc + 4 > len(self.prog):
            return 0
        val = int.from_bytes(self.prog[self.pc:self.pc+4], 'little')
        self.pc += 4
        return val

    def _read_u8(self) -> int:
        if self.pc >= len(self.prog):
            return 0
        val = self.prog[self.pc]
        self.pc += 1
        return val

    # Execute
    def execute(self, max_steps: int = 500000) -> None:
        steps = 0
        while not self.halted and not self.error_flag and steps < max_steps:
            if self.pc > 0xFEF or self.pc + 2 > len(self.prog):
                break
            opcode = self._read_u16()
            steps += 1

            # 0xdead: terminate program
            if opcode == 0xDEAD:
                break

            # 0x99a0..0x99a6: conditional jumps
            if 0x99A0 <= opcode <= 0x99A6:
                target = self._read_u16()
                take = False
                if opcode == 0x99A0:  # jmp
                    take = True
                elif opcode == 0x99A1:  # je
                    take = self.flag_Z is True
                elif opcode == 0x99A2:  # jne
                    take = self.flag_Z is False
                elif opcode == 0x99A3:  # z==0 and X==LT
                    take = (not self.flag_Z) and (self.flag_X == self.flag_LT)
                elif opcode == 0x99A4:  # X==LT
                    take = (self.flag_X == self.flag_LT)
                elif opcode == 0x99A5:  # X!=LT
                    take = (self.flag_X != self.flag_LT)
                elif opcode == 0x99A6:  # Z==1 or X!=LT
                    take = self.flag_Z or (self.flag_X != self.flag_LT)
                if take:
                    self.pc = target & 0xFFFF
                continue

            # 0x9900: CMP
            if opcode == 0x9900:
                dst_reg = self._read_u8()
                rhs = self._read_u32()
                flag = self._read_u8()
                if flag != 0:
                    # rhs refers to register id
                    rhs_reg = rhs & 0xFF
                    if not (1 <= rhs_reg <= 9):
                        self.error_flag = True
                        continue
                    rhs = self.regs[rhs_reg - 1]
                if not (1 <= dst_reg <= 9):
                    self.error_flag = True
                    continue
                lhs = self.regs[dst_reg - 1]
                res = (lhs - rhs) & 0xFFFFFFFF
                self.cmp_result = res
                self.flag_Z = (res == 0)
                self.flag_X = False
                self.flag_LT = (lhs & 0xFFFFFFFF) < (rhs & 0xFFFFFFFF)
                # Save detailed compare event
                evt = CompareEvent(
                    lhs=lhs & 0xFF,
                    rhs=rhs & 0xFF,
                    z=self.flag_Z,
                    lt=self.flag_LT,
                    last_input_index=self.last_input_index,
                )
                self.last_compare = evt
                self.compare_log.append(evt)
                # Heuristic: if compare used an input byte and failed, append a trace marker to stdout
                if self.last_input_index is not None and not self.flag_Z:
                    self.stdout += f"[cmp idx={self.last_input_index} lhs={lhs & 0xFF:02x} rhs={rhs & 0xFF:02x}]\n".encode()
                continue

            # 0xF00..0xF07 arithmetic / bitwise / mul/div/mod
            if 0xF00 <= opcode <= 0xF07:
                dst_reg = self._read_u8()
                val = self._read_u32()
                flag = self._read_u8()
                if flag != 0:
                    src_reg = val & 0xFF
                    if not (1 <= src_reg <= 9):
                        self.error_flag = True
                        continue
                    val = self.regs[src_reg - 1]
                if not (1 <= dst_reg <= 9):
                    self.error_flag = True
                    continue
                cur = self.regs[dst_reg - 1]
                if opcode == 0xF00:
                    cur = u32(cur + val)
                elif opcode == 0xF01:
                    cur = u32(cur - val)
                elif opcode == 0xF02:
                    cur = u32(cur | val)
                elif opcode == 0xF03:
                    cur = u32(cur & val)
                elif opcode == 0xF04:
                    cur = u32(cur ^ val)
                elif opcode == 0xF05:
                    cur = u32((cur * val) & 0xFFFFFFFF)
                elif opcode == 0xF06:
                    if val == 0:
                        self.error_flag = True
                        continue
                    cur = u32((cur // (val & 0xFFFFFFFF)))
                elif opcode == 0xF07:
                    cur = u32(cur % (val if val != 0 else 1))
                self.regs[dst_reg - 1] = cur
                continue

            # 0xF08 / 0xF09 shifts
            if opcode in (0xF08, 0xF09):
                dst_reg = self._read_u8()
                amt = self._read_u8()
                flag = self._read_u8()
                if flag != 0:
                    if not (1 <= amt <= 9):
                        self.error_flag = True
                        continue
                    amt = self.regs[amt - 1] & 0xFF
                if not (1 <= dst_reg <= 9):
                    self.error_flag = True
                    continue
                amt &= 0x1F
                if opcode == 0xF08:
                    self.regs[dst_reg - 1] = u32(self.regs[dst_reg - 1] << amt)
                else:
                    self.regs[dst_reg - 1] = u32(self.regs[dst_reg - 1] >> amt)
                continue

            # 0xD00 push (imm or reg)
            if opcode == 0xD00:
                val = self._read_u32()
                flag = self._read_u8()
                if flag != 0:
                    reg_id = val & 0xFF
                    if not (1 <= reg_id <= 9):
                        self.error_flag = True
                        continue
                    val = self.regs[reg_id - 1]
                # push: decrement SP by 4 then write
                if self.sp < 4:
                    self.error_flag = True
                    continue
                self.sp -= 4
                self._stack_write_u32(self.sp, val)
                continue

            # 0xD01 pop to register
            if opcode == 0xD01:
                reg_id = self._read_u8()
                if not (1 <= reg_id <= 9):
                    self.error_flag = True
                    continue
                if self.sp + 4 > 0x1000:
                    # underflow
                    self.error_flag = True
                    continue
                val = self._stack_read_u32(self.sp)
                self.sp += 4
                self.regs[reg_id - 1] = val
                continue

            # 0xD02 peek to register
            if opcode == 0xD02:
                reg_id = self._read_u8()
                if not (1 <= reg_id <= 9):
                    self.error_flag = True
                    continue
                if self.sp >= 0x1000:
                    self.error_flag = True
                    continue
                self.regs[reg_id - 1] = self._stack_read_u32(self.sp)
                continue

            # 0xC02: mov imm to reg
            if opcode == 0xC02:
                dst_reg = self._read_u8()
                imm = self._read_u32()
                if not (1 <= dst_reg <= 9):
                    self.error_flag = True
                    continue
                self.regs[dst_reg - 1] = u32(imm)
                continue

            # 0xC00 / 0xC01: mov reg->reg (and clear src if 0xC00)
            if opcode in (0xC00, 0xC01):
                dst_reg = self._read_u8()
                src_reg = self._read_u8()
                if not (1 <= dst_reg <= 9) or not (1 <= src_reg <= 9):
                    self.error_flag = True
                    continue
                self.regs[dst_reg - 1] = self.regs[src_reg - 1]
                if opcode == 0xC00:
                    self.regs[src_reg - 1] = 0
                continue

            # 0x5A0: store program string (UID, len, then len bytes copied from program)
            if opcode == 0x5A0:
                length = self._read_u8()
                uid = self._read_u32()
                off = self._store_find_uid(uid)
                if off == 0xEEEEFFFF:
                    self._store_add_string(uid, length, True)
                else:
                    self.error_flag = True
                continue

            # 0x5B0: load stored string by UID into input buffer
            if opcode == 0x5B0:
                uid = self._read_u32()
                self._store_load_to_input(uid)
                continue

            # 0x5B1: store input buffer as stored string under UID
            if opcode == 0x5B1:
                uid = self._read_u32()
                off = self._store_find_uid(uid)
                if off == 0xEEEEFFFF:
                    if self.input_len == 0:
                        self.error_flag = True
                    else:
                        self._store_add_string(uid, self.input_len, False)
                else:
                    self.error_flag = True
                continue

            # 0x5B2: load byte from input buffer at index (imm or reg) into R1
            if opcode == 0x5B2:
                idx = self._read_u8()
                flag = self._read_u8()
                if flag != 0:
                    if not (1 <= idx <= 9):
                        self.error_flag = True
                        continue
                    idx = self.regs[idx - 1] & 0xFF
                if idx >= self.input_len:
                    self.error_flag = True
                    continue
                self.regs[0] = self.input_buf[idx]
                self.last_input_index = idx
                continue

            # 0x5B3: load byte from stored string by UID at offset (imm or reg) into R1
            if opcode == 0x5B3:
                uid = self._read_u32()
                idx = self._read_u8()
                flag = self._read_u8()
                off = self._store_find_uid(uid)
                if off == 0xEEEEFFFF:
                    self.error_flag = True
                    continue
                if flag != 0:
                    if not (1 <= idx <= 9):
                        self.error_flag = True
                        continue
                    idx = self.regs[idx - 1] & 0xFF
                strlen = self.stored[off]
                if idx > strlen:
                    self.error_flag = True
                    continue
                base = off + 1
                self.regs[0] = self.stored[base + idx]
                continue

            # 0xFFFF: native syscall based on R1
            if opcode == 0xFFFF:
                call_id = self.regs[0] & 0xFFFFFFFF
                if call_id == 0x01:
                    # read input (simulate: input already present in self.input_buf and self.input_len)
                    # In C, max len comes from low byte of R2; we'll cap to existing len
                    maxlen = (self.regs[1] & 0xFF)
                    if maxlen == 0:
                        self.error_flag = True
                    else:
                        if self.input_len > 0:
                            # ensure NUL terminator behavior not needed here
                            pass
                        else:
                            # if no input provided, set error
                            self.error_flag = True
                elif call_id == 0x02:
                    # write/print
                    # If R13(low byte) == 1 -> print from input_buf of length (low byte of pc), else by UID in R2
                    length_hint = self.pc & 0xFF
                    src_flag = (self.regs[3] & 0xFF)  # using R4 low byte as cVar2 proxy (offset +0x0c)
                    if src_flag == 1:
                        n = length_hint if 0 < length_hint <= self.input_len else self.input_len
                        self.stdout += self.input_buf[:n]
                    else:
                        uid = self.regs[1]
                        off = self._store_find_uid(uid)
                        if off == 0xEEEEFFFF:
                            self.error_flag = True
                        else:
                            slen = self.stored[off]
                            n = length_hint if (0 < length_hint <= slen) else slen
                            self.stdout += self.stored[off+1:off+1+n]
                elif call_id == 0x03:
                    # load stored UID in R2 into input buffer
                    uid = self.regs[1]
                    self._store_load_to_input(uid)
                elif call_id == 0x04:
                    # register dump (ignore)
                    pass
                elif call_id == 0x05:
                    # stack dump (ignore)
                    pass
                elif call_id == 0x06:
                    # stored strings dump (ignore)
                    pass
                elif call_id == 0xFF:
                    self.halted = True
                else:
                    # unknown native -> error
                    self.error_flag = True
                continue

            # Unknown opcode -> error to avoid infinite loop
            # But keep permissive: just mark error and stop
            # print(f"[VM] Unknown opcode 0x{opcode:04x} at PC=0x{self.PC:04x}")
            self.error_flag = True
            break

    def set_input(self, data: bytes) -> None:
        n = min(len(data), len(self.input_buf))
        self.input_buf[:n] = data[:n]
        self.input_len = n


def run_program_with_input(program: bytes, user_input: bytes, max_steps: int = 200000) -> Tuple[AshfaqVM, Optional[CompareEvent]]:
    vm = AshfaqVM(program)
    vm.set_input(user_input)
    vm.execute(max_steps=max_steps)
    return vm, vm.last_compare


if __name__ == "__main__":
    with open('chal.ashfaq', 'rb') as f:
        program = f.read()

    # Try running once with a dummy input and print any output
    seed = b"PCC{test_input_place_holder________________________}"
    vm, cmp_evt = run_program_with_input(program, seed)
    out = vm.stdout.decode('utf-8', errors='replace')
    print("\n[VM OUTPUT]\n" + out)
    if vm.error_flag:
        print("[!] VM encountered an error during execution.")
    if cmp_evt:
        print(f"[last compare] lhs=0x{cmp_evt.lhs:02x} rhs=0x{cmp_evt.rhs:02x} z={cmp_evt.z} lt={cmp_evt.lt} idx={cmp_evt.last_input_index}")
