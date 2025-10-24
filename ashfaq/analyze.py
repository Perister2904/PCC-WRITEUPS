#!/usr/bin/env python3
"""
Quick analysis script to extract key information from the bytecode.
Shows embedded strings and their UIDs.
"""

with open('chal.ashfaq', 'rb') as f:
    data = f.read()

print("[*] AshfaqVM Bytecode Analysis")
print(f"[*] File size: {len(data)} bytes")

# Find stored strings (format: 0xa0 <type> <len> <data>)
print("\n[*] Searching for stored strings (opcode 0xa0)...")

i = 0
string_count = 0
while i < len(data) - 6:
    if data[i:i+2] == b'\xa0\x05':  # 0xa0 = store string, 0x05 = type
        string_count += 1
        length = data[i+2]
        uid = int.from_bytes(data[i+3:i+7], 'little')
        string_data = data[i+7:i+7+length]
        
        print(f"\n[String {string_count}]")
        print(f"  Offset: 0x{i:04x}")
        print(f"  UID: 0x{uid:08x}")
        print(f"  Length: {length} bytes")
        
        try:
            decoded = string_data.decode('ascii', errors='replace')
            print(f"  ASCII: {decoded}")
        except:
            pass
        
        print(f"  Hex: {string_data.hex()}")
        
        # Skip to next potential string
        i += 7 + length
    else:
        i += 1

# Look for key constants
print("\n[*] Searching for magic constants...")
if b'\xef\xbe\xad\xde' in data:
    offset = data.find(b'\xef\xbe\xad\xde')
    print(f"  0xDEADBEEF found at offset 0x{offset:04x}")

if b'\xad\xde\xad\xde' in data:
    offset = data.find(b'\xad\xde\xad\xde')
    print(f"  0xDEADDEAD found at offset 0x{offset:04x}")

print("\n[*] Analysis complete!")
