#!/usr/bin/env python3
"""
AshfaqVM Flag Recovery Script
Brute forces each character position by running the VM emulator
and checking if the comparison at that index succeeds.
"""
import sys
sys.path.insert(0, '.')

# Import the VM emulator
from full_vm import AshfaqVM

# Load the bytecode
with open('chal.ashfaq', 'rb') as f:
    program = f.read()

def find_char_at_position(idx, known_prefix=b''):
    """
    Brute force the character at position idx by trying all 256 possible bytes.
    Returns the character that causes the CMP at index idx to succeed (flag_Z=True).
    """
    # Start with known characters
    base = bytearray(known_prefix)
    
    # Pad to full length (we know it's 71 bytes from analysis)
    while len(base) < 71:
        base.append(ord('X'))
    
    for c in range(256):
        test_input = bytearray(base)
        test_input[idx] = c
        
        # Run VM with this test input
        vm = AshfaqVM(program)
        vm.set_input(bytes(test_input))
        
        try:
            vm.execute(max_steps=500000)
        except:
            continue
        
        # Check all comparison events
        for evt in vm.compare_log:
            if evt.last_input_index == idx and evt.z:  # z=True means equal
                return chr(c) if 32 <= c < 127 else f'\\x{c:02x}'
    
    return '?'

def recover_flag():
    """
    Recover the full flag by brute forcing each position sequentially.
    """
    flag = bytearray()
    
    # We know it starts with "PCC{" from the challenge context
    known_start = b'PCC{'
    flag.extend(known_start)
    
    print("[*] Starting flag recovery...")
    print(f"[*] Known prefix: {known_start.decode()}")
    
    # Recover remaining characters
    for idx in range(len(flag), 71):
        print(f"[*] Progress: {idx}/71")
        
        char = find_char_at_position(idx, flag)
        flag.append(ord(char) if len(char) == 1 else int(char[2:], 16))
        
        print(f"[{idx}] = '{char}'")
        
        # Show current flag state every 5 characters
        if (idx + 1) % 5 == 0:
            print(f"    Current: {flag.decode('ascii', errors='replace')}")
    
    print(f"\n[+] Flag recovered: {flag.decode('ascii', errors='replace')}")
    return flag.decode('ascii', errors='replace')

if __name__ == "__main__":
    recovered = recover_flag()
    
    # Save to file
    with open('flag.txt', 'w') as f:
        f.write(recovered)
    
    print(f"\n[+] Flag saved to flag.txt")
    print(f"[+] Verify with: echo '{recovered}' | ./ashfaq-vm chal.ashfaq")
