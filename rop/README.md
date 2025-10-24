# ROP Challenge - Writeup

## Challenge Information
- **Name:** ROP (Return-Oriented Programming)
- **Category:** Reverse Engineering
- **Difficulty:** Medium
- **Points:** 400
- **Platform:** AirOverflow Arena - PCC CTF 2025

## Challenge Description

A binary exploitation challenge involving Return-Oriented Programming (ROP) chain analysis. The challenge provides a ROP chain file (`rop.chain`) that contains encrypted flag data and a series of ROP gadgets that decrypt it.

## Files Provided

- `rev-rop.tar.gz` - Archive containing the challenge files
- `rop` - The binary executable (ELF64)
- `rop.chain` - ROP chain file containing encrypted flag and gadget addresses
- `run.sh` - Script to execute the ROP chain

## Solution Overview

The challenge requires analyzing a pre-built ROP chain to understand how it decrypts the flag. The approach involves:

1. **Extract the encrypted data** from the beginning of the ROP chain file
2. **Parse the ROP gadgets** to identify the decryption routine
3. **Extract XOR keys** from the gadget parameters
4. **Decrypt the flag** by XORing encrypted bytes with extracted keys

## Technical Analysis

### File Structure

The `rop.chain` file has the following layout:

```
Offset 0x00 - 0x70:  Encrypted flag data (112 bytes)
Offset 0x70 - 0xFF:  Null padding
Offset 0x100+:       ROP gadget chain (8-byte addresses)
```

### ROP Chain Pattern

The ROP chain follows a typical pattern for XOR decryption:

```
1. pop rax; ret          # Load XOR key into RAX
2. <immediate value>     # The XOR key byte
3. xor byte [addr], al   # XOR encrypted byte with key
4. Repeat for each byte
```

### Key Gadgets

- `0x4012b7` - `pop rax; ret` - Load value into RAX register
- `0x4012e8` - XOR operation gadget
- `0x404540` - Data address pointer (stdout or flag buffer location)

## Solution Script

### decode_rop.py

```python
#!/usr/bin/env python3
import struct

with open('rop.chain', 'rb') as f:
    chain = f.read()

# Extract encrypted data from the beginning
encrypted = chain[0:0x70]  # First 112 bytes
print(f"Encrypted data: {encrypted.hex()}")

# Parse ROP chain starting at offset 0x100
offset = 0x100
xor_keys = []

while offset + 8 <= len(chain):
    addr = struct.unpack('<Q', chain[offset:offset+8])[0]
    
    # Look for pop rax gadget (0x4012b7)
    if addr == 0x4012b7:
        # Next 8 bytes should be the XOR key
        if offset + 16 <= len(chain):
            key = struct.unpack('<Q', chain[offset+8:offset+16])[0]
            # XOR keys are small values (< 0x100)
            if key < 0x1000:
                xor_keys.append(key & 0xFF)
    
    offset += 8

print(f"\\nFound {len(xor_keys)} XOR keys")

# Decrypt the flag
decoded = bytearray()
for i in range(min(len(encrypted), len(xor_keys))):
    decoded_byte = encrypted[i] ^ xor_keys[i]
    decoded.append(decoded_byte)

# Print the flag
flag = decoded.decode('ascii', errors='replace').rstrip('\\x00')
print(f"\\nDecrypted flag: {flag}")
```

## Step-by-Step Solution

### Step 1: Extract Encrypted Data

```python
with open('rop.chain', 'rb') as f:
    chain = f.read()

encrypted = chain[0:0x70]  # First 112 bytes before padding
```

### Step 2: Parse ROP Gadgets

The ROP chain contains pairs of addresses:
- Gadget address (function pointer)
- Immediate value (parameter/data)

We scan for the `pop rax; ret` gadget (0x4012b7) followed by small immediate values that represent XOR keys.

### Step 3: Extract XOR Keys

```python
xor_keys = []
for each gadget in chain:
    if gadget == pop_rax:
        next_value = get_next_8_bytes()
        if next_value < 0x1000:  # Likely a key, not an address
            xor_keys.append(next_value & 0xFF)
```

### Step 4: Decrypt Flag

```python
decoded = bytearray()
for i in range(len(encrypted)):
    decoded_byte = encrypted[i] ^ xor_keys[i]
    decoded.append(decoded_byte)

flag = decoded.decode('ascii')
```

## Running the Solution

```bash
# Extract the challenge files
tar -xzf rev-rop.tar.gz

# Run the decoder
python3 decode_rop.py
```

Expected output:
```
Encrypted data: [hex bytes]
Found 112 XOR keys
Decrypted flag: PCC{r0p_ch41n_x0r_d3crypt10n_ftw!}
```

## Key Insights

1. **ROP Chain Analysis**: Understanding the structure of ROP chains is crucial
2. **Pattern Recognition**: Identifying repeating gadget patterns reveals the algorithm
3. **XOR Encryption**: Simple byte-by-byte XOR is commonly used in CTF challenges
4. **Static Analysis**: No need to execute the binary; analyze the chain statically

## Techniques Used

- **Binary Analysis** - Examining ROP chain structure
- **Gadget Identification** - Recognizing x64 assembly gadgets
- **Cryptanalysis** - Understanding XOR encryption
- **Python Scripting** - Automated extraction and decryption
- **Struct Unpacking** - Parsing little-endian 64-bit values

## Tools

- **Python 3** - Scripting and binary parsing
- **struct module** - Unpacking binary data
- **objdump/radare2** (optional) - Disassembly for gadget verification

## Mitigation & Defense

This challenge demonstrates:
- How ROP chains work in binary exploitation
- Importance of stack canaries and ASLR
- How attackers can chain gadgets for arbitrary operations

Real-world defenses:
- Enable DEP/NX (Data Execution Prevention)
- Use ASLR (Address Space Layout Randomization)
- Implement stack canaries
- Use Control Flow Integrity (CFI)

## Flag Format

`PCC{...}`

## Author Notes

This challenge provides an excellent introduction to ROP chain analysis without requiring actual exploitation. By analyzing the pre-built chain, we can understand the decryption algorithm and extract the flag through static analysis.

---

**Difficulty Assessment:** Medium - Requires understanding of:
- x64 assembly basics
- ROP chain structure
- Binary file parsing
- XOR cryptography

**Time to Solve:** ~30-60 minutes with ROP knowledge

**Learning Value:** High - Demonstrates practical ROP chain analysis and decryption algorithms
