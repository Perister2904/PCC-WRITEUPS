import torch, numpy as np, re

model = torch.load('model.pt', map_location='cpu')
pat = re.compile(br'PCC\{[^}]{0,400}\}')

# Get tensors list
if isinstance(model, dict):
    tensors = [(k, v) for k, v in model.items() if isinstance(v, torch.Tensor)]
else:
    tensors = [(n, p.detach()) for n, p in model.named_parameters()]

# Helper: convert bits -> bytes

def bits_to_bytes(bits, lsb_first=True):
    out = bytearray()
    for i in range(0, len(bits) - 7, 8):
        b = 0
        if lsb_first:
            for j in range(8):
                b |= (bits[i+j] & 1) << j
        else:
            for j in range(8):
                b = ((b << 1) | (bits[i+j] & 1)) & 0xFF
        out.append(b)
    return bytes(out)

# 7-bit packing

def bits_to_7bit_ascii(bits):
    out = bytearray()
    acc = 0; nbits = 0
    for bit in bits:
        acc |= (bit & 1) << nbits
        nbits += 1
        if nbits == 7:
            out.append(acc & 0x7F)
            acc = 0; nbits = 0
    return bytes(out)

found_any = False

for name, t in tensors:
    arr = t.detach().cpu().numpy().astype(np.float32).ravel()
    u32 = arr.view(np.uint32)
    # Try bit positions across 0..22 (mantissa) and 23..30 (exponent) and 31 (sign)
    for bitpos in range(0, 32):
        bits = ((u32 >> bitpos) & 1).astype(np.uint8).tolist()
        # Try multiple packings
        for lsb_first in (True, False):
            data8 = bits_to_bytes(bits, lsb_first=lsb_first)
            m = pat.search(data8)
            if m:
                print(f'[{name}] bit{bitpos} -> 8bit lsb_first={lsb_first}:', m.group(0).decode('ascii','ignore'))
                found_any = True
        data7 = bits_to_7bit_ascii(bits)
        m = pat.search(data7)
        if m:
            print(f'[{name}] bit{bitpos} -> 7bit:', m.group(0).decode('ascii','ignore'))
            found_any = True

# Also try taking k LSBs (k=2..5) per value and concatenating
for name, t in tensors:
    arr = t.detach().cpu().numpy().astype(np.float32).ravel()
    u32 = arr.view(np.uint32)
    for k in range(2, 6):
        bits = []
        for v in u32:
            for b in range(k):
                bits.append((v >> b) & 1)
        for lsb_first in (True, False):
            data8 = bits_to_bytes(bits, lsb_first=lsb_first)
            m = pat.search(data8)
            if m:
                print(f'[{name}] {k}-LSBs -> 8bit lsb_first={lsb_first}:', m.group(0).decode('ascii','ignore'))
                found_any = True
        data7 = bits_to_7bit_ascii(bits)
        m = pat.search(data7)
        if m:
            print(f'[{name}] {k}-LSBs -> 7bit:', m.group(0).decode('ascii','ignore'))
            found_any = True

print('Found any:', found_any)
