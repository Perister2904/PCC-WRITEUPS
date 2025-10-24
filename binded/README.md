# "binded" Forensics CTF – Full Write‑up

Flag: `PCC{WhY_Y0U_D1STURB1N_H4CK3R_69???}`

## TL;DR
- Opened `challenge.pcap` and found a short HTTP session with Base64 payloads.
- Server set a cookie: `cookie=tf/+HtmutvI=`. Base64‑decoding gives 8 bytes: `b5fffe1ed9aeb6f2`.
- Every HTTP JSON body (both POSTs and 200 responses) began with the same 8‑byte ciphertext prefix `bcb474e479c1fbc2`.
- Treat each body as `ciphertext = iv(8 bytes) || data`, and decrypt via DES‑CBC with:
  - key = decode_base64(cookie)
  - iv   = first 8 bytes of ciphertext
  - pt   = DES-CBC(key, iv).decrypt(ciphertext[8:]) then unpad (block size 8)
- The decrypted transcript includes Linux command outputs and the flag.

## Files in this folder
- `challenge.pcap` – the capture provided by the challenge.
- `decrypt_des_sessions.py` – final solver that extracts, decrypts, and prints the transcript + flag.
- `try_des.py` – quick probe that revealed DES‑CBC worked and produced readable text.
- (Aux) Various exploratory scripts: `analyze_pcap.py`, `extract_http*.py`, `check_*`, etc.

## How to reproduce (Windows PowerShell)

```powershell
# 1) Ensure Python venv is active or use the full path to your venv's python
c:/Users/haryp/Downloads/forensics/.venv/Scripts/python.exe -m pip install scapy pycryptodome

# 2) Run the final decryptor
c:/Users/haryp/Downloads/forensics/.venv/Scripts/python.exe decrypt_des_sessions.py
```

Expected tail of output:
```
[POST 8]:
echo 'PCC{WhY_Y0U_D1STURB1N_H4CK3R_69???}'

[RESP 8]:
PCC{WhY_Y0U_D1STURB1N_H4CK3R_69???}

FLAG FOUND: PCC{WhY_Y0U_D1STURB1N_H4CK3R_69???}
```

## Reasoning and steps

1) Initial triage
- Scapy pass: counted TCP/UDP/ICMP/DNS and looked for HTTP. HTTP request/response pairs had Base64 bodies like `vLR05HnB+8I...`.
- Noticed every response Base64 decodes to bytes beginning with the same 8‑byte prefix `bc b4 74 e4 79 c1 fb c2`.
- Server sends a Set-Cookie header: `cookie=tf/+HtmutvI=`.
  - Base64 decode → key bytes: `b5 ff fe 1e d9 ae b6 f2` (8 bytes) → strong hint for DES, not AES.

2) False starts and why they failed
- XOR attempts (repeating key): produced plausible but garbled ASCII blocks; not consistent plain text.
- AES (ECB/CBC) with repeated cookie as 16/24/32: output length not a multiple of 16 and gibberish; doesn’t match block structure we observed.
- RC4 guess: unnecessary after confirming DES block properties.
- DNS/ICMP: background/system noise, not part of the flag channel.

3) DES hypothesis and validation
- DES uses 8‑byte keys and 8‑byte blocks. The 8‑byte stable prefix strongly suggested it’s the IV.
- Tested DES‑ECB and DES‑CBC quickly (`try_des.py`).
  - DES‑ECB gave junk.
  - DES‑CBC using IV = first 8 bytes of ciphertext immediately yielded readable text:
    - Resp1 → `root`
    - Resp3 → `/root`
    - Longer response → `ps aux` table, etc.

4) Full session decryption
- Implemented `decrypt_des_sessions.py`:
  - Extract cookie from HTTP `Set-Cookie`.
  - Gather POST bodies and 200 OK bodies in order.
  - For each body:
    - `ct = base64_decode(body)`
    - `iv = ct[:8]`
    - `pt = DES-CBC(key, iv).decrypt(ct[8:])` and `unpad` (block size 8)
  - Print transcript; search for `PCC{.*}`.
- The final POST echoes the flag and the final response returns the flag.

## Key technical details (cheatsheet)
- Cookie → key: `tf/+HtmutvI=` → base64 → `0xb5 0xff 0xfe 0x1e 0xd9 0xae 0xb6 0xf2`.
- Ciphertext layout per HTTP body:
  - `ciphertext = iv(8B) || data(8N)`
  - Algorithm: `DES-CBC` (key=cookie_bytes, iv=ciphertext[:8])
  - Padding: PKCS#7 on 8‑byte blocks (unpad after decrypt)

Minimal Python snippet:
```python
import base64
from Crypto.Cipher import DES
from Crypto.Util.Padding import unpad

def des_cbc_cookie_decrypt(b64_body: str, key_b64: str) -> bytes:
    key = base64.b64decode(key_b64)
    ct  = base64.b64decode(b64_body)
    iv, rest = ct[:8], ct[8:]
    pt = DES.new(key, DES.MODE_CBC, iv).decrypt(rest)
    try:
        pt = unpad(pt, 8)
    except Exception:
        pass
    return pt
```

## Why the name “binded”?
- The IV is “bound” to each ciphertext block (prepended), and the cookie is “bound” as the symmetric key. The interaction mimics a simple DES‑CBC command‑and‑control.

## Verification
- Scripts executed successfully in the provided venv.
- Final output includes the plaintext transcript and the flag.

## Flag
`PCC{WhY_Y0U_D1STURB1N_H4CK3R_69???}`
