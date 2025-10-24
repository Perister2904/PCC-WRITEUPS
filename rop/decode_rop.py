#!/usr/bin/env python3

with open('rop.chain', 'rb') as f:
    data = f.read()

# Skip to ROP chain (starts at offset 0x100)
offset = 0x100
gadgets = []

while offset < len(data):
    if offset + 8 <= len(data):
        addr = int.from_bytes(data[offset:offset+8], 'little')
        if addr != 0:
            gadgets.append((offset, addr))
        offset += 8
    else:
        break

# Print addresses
print("ROP Chain gadgets:")
for off, addr in gadgets[:50]:  # Print first 50
    print(f"Offset 0x{off:04x}: 0x{addr:016x}")
