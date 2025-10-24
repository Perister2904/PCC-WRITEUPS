from scapy.all import *
import base64
from Crypto.Cipher import DES
from Crypto.Util.Padding import unpad

pcap = 'challenge.pcap'
packets = rdpcap(pcap)

# Extract DES key from cookie header
key = None
for pkt in packets:
    if pkt.haslayer(TCP) and pkt.haslayer(Raw):
        data = pkt[Raw].load
        if b'Set-Cookie: cookie=' in data:
            cookie = data.split(b'Set-Cookie: cookie=')[1].split(b';')[0]
            key = base64.b64decode(cookie)
            break

if not key:
    print('Cookie key not found!')
    raise SystemExit(1)

print(f'DES key (hex): {key.hex()}')

# Collect in-order POST bodies and 200 OK bodies
posts = []
resps = []
for pkt in packets:
    if pkt.haslayer(TCP) and pkt.haslayer(Raw):
        payload = pkt[Raw].load
        # POST bodies
        if b'POST /api/analytics' in payload and b'Content-Length:' in payload:
            parts = payload.split(b'\r\n\r\n', 1)
            if len(parts) == 2 and parts[1].strip():
                posts.append(parts[1].strip())
        # Responses
        elif b'HTTP/1.1 200 OK' in payload and b'Content-Type: application/json' in payload:
            parts = payload.split(b'\r\n\r\n', 1)
            if len(parts) == 2 and parts[1].strip():
                resps.append(parts[1].strip())

print(f'Collected {len(posts)} POST bodies and {len(resps)} responses')

# DES-CBC decryption using IV in first 8 bytes of ciphertext
def des_cbc_iv_c0(b64: bytes) -> bytes:
    try:
        ct = base64.b64decode(b64)
    except Exception:
        return b''
    if len(ct) < 8:
        return b''
    iv, rest = ct[:8], ct[8:]
    dec = DES.new(key, DES.MODE_CBC, iv).decrypt(rest)
    try:
        dec = unpad(dec, 8)
    except Exception:
        pass
    return dec

# Decrypt everything and scan for flag
flag = None
print('\n===== Decrypted Transcript =====')
for i in range(max(len(posts), len(resps))):
    if i < len(posts):
        p = des_cbc_iv_c0(posts[i])
        if p:
            print(f"\n[POST {i+1}]:")
            print(p.decode('utf-8', errors='replace'))
            if b'PCC{' in p:
                s = p.find(b'PCC{'); e = p.find(b'}', s) + 1
                flag = p[s:e].decode('utf-8', errors='ignore')
    if i < len(resps):
        r = des_cbc_iv_c0(resps[i])
        if r:
            print(f"\n[RESP {i+1}]:")
            print(r.decode('utf-8', errors='replace'))
            if not flag and b'PCC{' in r:
                s = r.find(b'PCC{'); e = r.find(b'}', s) + 1
                flag = r[s:e].decode('utf-8', errors='ignore')

print('\n================================')
if flag:
    print(f'\nFLAG FOUND: {flag}')
else:
    print('Flag not found in transcript. Trying a broader scan...')
    # broader scan across all decrypted buffers
    for buf in [des_cbc_iv_c0(b) for b in posts + resps]:
        if b'PCC{' in buf:
            s = buf.find(b'PCC{'); e = buf.find(b'}', s) + 1
            print(f"FLAG FOUND: {buf[s:e].decode('utf-8', errors='ignore')}")
            break
