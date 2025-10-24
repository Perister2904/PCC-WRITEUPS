# AshfaqVM Challenge Solution

## Challenge Overview
AshfaqVM is a custom virtual machine that loads a bytecode file (`chal.ashfaq`) and executes it. The VM prompts for a flag input and validates it character-by-character against an encrypted stored string.

## Files Included
- `chal.ashfaq` - The bytecode program containing encrypted flag
- `ashfaq-vm` - The compiled VM binary (ELF64)
- `ashfaq-vm.c` - Decompiled source code (from Ghidra)
- `full_vm.py` - Python emulator implementing the VM instruction set
- `brute_decrypt.py` - Final solution script that recovers the flag
- `README.md` - This file

## Solution Approach

### Step 1: Understanding the VM
The VM has:
- 8 general-purpose registers (R1-R8)
- Stack memory (0x1000 bytes)
- String storage area for embedded strings
- Input buffer for user input
- Various opcodes for arithmetic, comparison, jumps, etc.

Key opcodes discovered:
- `0xC02` - Load immediate into register
- `0x9900` - Compare (CMP) operation
- `0x99A1` - Jump if equal (JE)
- `0x5B0` - Load stored string by UID
- `0x5B2` - Load byte from input buffer
- `0x5B3` - Load byte from stored string
- `0xF04` - XOR operation

### Step 2: Analyzing the Bytecode
The bytecode contains several embedded strings:
1. "PCC 2025 - AshfaqVM" (banner)
2. "Please enter the flag:" (prompt)
3. "Correct!" (success message)
4. "Nope!" (failure message)
5. **Encrypted flag data** (UID=0xf746f746, 71 bytes)

### Step 3: Identifying the Validation Loop
The VM performs these operations in a loop for each character:
1. Load a byte from user input at index `i`
2. Load the corresponding byte from the encrypted stored string
3. Apply transformations (XOR with constants)
4. Compare transformed byte with user input byte
5. If match, continue; if not, print "Nope!" and exit

### Step 4: Building a Python Emulator
Created `full_vm.py` - a full Python implementation of the VM that:
- Parses and executes bytecode instructions
- Implements all major opcodes
- Logs comparison events (CMP operations)
- Captures which byte index is being validated

### Step 5: Brute Force Solution
Since the VM compares each input byte individually, we can brute-force byte-by-byte:

```python
def find_char_at_position(idx):
    for c in range(256):
        test_input = bytearray(b'PCC{' + b'X' * 67)  # 71 bytes total
        test_input[idx] = c
        
        vm = AshfaqVM(program)
        vm.set_input(bytes(test_input))
        vm.execute(max_steps=500000)
        
        # Check if this position passed validation
        for evt in vm.compare_log:
            if evt.last_input_index == idx and evt.z:  # z=True means equal
                return chr(c)
    return '?'
```

For each position 0-70, we:
1. Create test input with known format `PCC{...}`
2. Set one byte to each possible value (0-255)
3. Run the VM emulator
4. Check if the comparison at that index resulted in equality (flag_Z=True)
5. If yes, that's the correct character

### Step 6: Extracting the Flag
Running `brute_decrypt.py` recovers the flag byte-by-byte:
```
[0] = 'P'
[1] = 'C'
[2] = 'C'
[3] = '{'
[4] = '1'
[5] = 'f'
...
```

## Key Insights
1. **Per-byte validation**: The VM validates each character independently, making it vulnerable to brute force
2. **Stored strings**: The encrypted flag is embedded in the bytecode as a stored string
3. **Comparison tracing**: By logging CMP operations and their results, we can determine when we've found the correct byte
4. **No complex crypto**: Despite appearing complex, the validation is deterministic and can be reversed through execution tracing

## Running the Solution
```bash
# Run the brute force decryptor
python -X utf8 brute_decrypt.py

# Or verify with the actual VM
echo "PCC{recovered_flag_here}" | ./ashfaq-vm chal.ashfaq
```

## Flag Format
`PCC{...}` - 71 characters total

## Techniques Used
- Static analysis (decompilation with Ghidra)
- Dynamic analysis (VM emulation)
- Instrumentation (comparison event logging)
- Brute force (per-byte exhaustive search)
- Reverse engineering (understanding custom VM architecture)

## Time Complexity
- O(256 × flag_length) = O(256 × 71) ≈ 18,000 VM executions
- Each execution is fast (~0.01s), total time ~3-5 minutes

## Author
Challenge: AshfaqVM (PCC CTF 2025)
Solution: Created using Python VM emulation and brute force
