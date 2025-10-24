#!/usr/bin/env python3
"""
Ultimate approach: manually decrypt the stored string using the exact sequence from bytecode.
Looking at 0x0183 and 0x018b, the bytecode does:
- Load encrypted byte from stored string into R1
- XOR R1 with something (0xdeadbeef or 0xdeaddead)
- Compare R1 to input

Let me trace the exact instruction sequence.
"""

with open('chal.ashfaq', 'rb') as f:
    data = f.read()

# Extract encrypted flag
encrypted_flag = None
offset = 0
while offset < len(data):
    if data[offset] == 0xa0:
        string_type = data[offset+1]
        length = data[offset+2]
        if offset + 3 + 4 + length > len(data):
            break
        uid = int.from_bytes(data[offset+3:offset+7], 'little')
        content = data[offset+7:offset+7+length]
        if uid == 0xf746f746:
            encrypted_flag = content
            break
        offset += 7 + length
    else:
        offset += 1

if not encrypted_flag:
    print("[!] Encrypted flag not found")
    exit(1)

print(f"[*] Encrypted flag: {encrypted_flag.hex()}")

# From the bytecode trace:
# At 0x0183: XOR instruction with params 0f 03 03 00 00 00
# At 0x018b: XOR instruction with params 0f 04 04 00 00 00
# These are 0xF04 opcodes (XOR reg, val, flag)

# At 0x017E: MOV R2, 0xdeadbeef
# At 0x019E: MOV R2, 0xdeaddead

# The sequence seems to be:
# for each byte index:
#   R1 = encrypted[index] (via 0x5B3)
#   R2 = 0xdeadbeef
#   R3 = R3 XOR R3 (result: 0 or R3?)
#   R4 = R4 XOR R4 (result: 0 or R4?)
#   Then later:
#   R2 = 0xdeaddead
#   R3 = R3 XOR R3
#   R4 = R4 XOR R4

# This is confusing. Let me just try all reasonable transformations systematically.

key1_bytes = 0xdeadbeef.to_bytes(4, 'little')
key2_bytes = 0xdeaddead.to_bytes(4, 'little')

# Extended test suite
tests = []

# Single-key XOR variations
for key_name, key in [("key1", key1_bytes), ("key2", key2_bytes)]:
    tests.append((f"XOR {key_name}", lambda enc, i, k=key: enc ^ k[i % 4]))
    tests.append((f"XOR {key_name} + index", lambda enc, i, k=key: enc ^ k[i % 4] ^ (i & 0xFF)))
    tests.append((f"XOR {key_name} + (index*2)", lambda enc, i, k=key: enc ^ k[i % 4] ^ ((i*2) & 0xFF)))

# Double-key XOR variations
tests.append(("XOR key1 then key2", lambda enc, i: enc ^ key1_bytes[i % 4] ^ key2_bytes[i % 4]))
tests.append(("XOR key2 then key1", lambda enc, i: enc ^ key2_bytes[i % 4] ^ key1_bytes[i % 4]))
tests.append(("XOR key1 then key2 + index", lambda enc, i: enc ^ key1_bytes[i % 4] ^ key2_bytes[i % 4] ^ (i & 0xFF)))

# Alternating keys
tests.append(("Alternate key1/key2", lambda enc, i: enc ^ (key1_bytes if i % 2 == 0 else key2_bytes)[i % 4]))

# XOR with combined 8-byte key
combined_key = key1_bytes + key2_bytes
tests.append(("XOR 8-byte combined", lambda enc, i: enc ^ combined_key[i % 8]))

print(f"\n[*] Testing {len(tests)} decryption methods...\n")

for desc, transform in tests:
    decrypted = bytearray()
    for i in range(len(encrypted_flag)):
        decrypted.append(transform(encrypted_flag[i], i))
    
    try:
        decoded = bytes(decrypted).decode('ascii', errors='ignore')
        # Check for PCC{ at start
        if decoded.startswith('PCC{'):
            print(f"[!!!] {desc}:")
            print(f"      FLAG: {decoded}")
            print()
            
            # Verify with VM
            from full_vm import AshfaqVM
            vm = AshfaqVM(data)
            vm.set_input(decrypted)
            vm.execute(max_steps=500000)
            out = vm.stdout.decode('utf-8', errors='replace')
            if "Correct" in out:
                print(f"[SUCCESS] FLAG VERIFIED!")
                break
    except:
        pass

print("[*] No valid flag found with standard transformations")
print("[*] Bruteforcing flag character-by-character...")

# Brute force each position
flag = bytearray(b"?" * 72)  # Initialize with placeholders
from full_vm import AshfaqVM

for target_idx in range(len(encrypted_flag)):
    if target_idx % 10 == 0:
        print(f"[*] Progress: {target_idx}/{len(encrypted_flag)}")
    
    # Try all printable ASCII
    for test_char in range(32, 127):
        test_input = bytearray(flag)
        test_input[target_idx] = test_char
        
        vm = AshfaqVM(data)
        vm.set_input(bytes(test_input))
        vm.execute(max_steps=500000)
        
        # Check if compare at this index succeeded
        if vm.compare_log:
            for evt in vm.compare_log:
                if evt.last_input_index == target_idx and evt.z:  # Match!
                    flag[target_idx] = test_char
                    print(f"    [{target_idx}] = '{chr(test_char)}'")
                    break
            if flag[target_idx] != ord('?'):
                break

# Show final result
try:
    final_flag = bytes(flag).decode('ascii', errors='replace')
    print(f"\n[!!!] RECOVERED FLAG: {final_flag}")
    
    # Verify
    vm_verify = AshfaqVM(data)
    vm_verify.set_input(flag)
    vm_verify.execute(max_steps=500000)
    out = vm_verify.stdout.decode('utf-8', errors='replace')
    print(f"\n[VM OUTPUT]:\n{out}")
    if "Correct" in out:
        print(f"\n[SUCCESS] FLAG VERIFIED: {final_flag}")
except Exception as e:
    print(f"[!] Error: {e}")
    print(f"[!] Hex: {flag.hex()}")
