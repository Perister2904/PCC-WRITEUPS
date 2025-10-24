# NeuroSteg (CTF) — Writeup

Flag format: `PCC{.*?}`

Final flag: `PCC{st3g0_in_th3_w31ghts_OG}`

---

## Overview
We were given a small PyTorch model (`model.pt`). The prompt hinted that the model "behaves normally" but hides a secret in its parameters. The correct flag pattern is `PCC{...}`.

This writeup summarizes everything we tried and the exact method that produced the valid PCC flag.

---

## Repo contents (key scripts)
- `bitplane_hunt_pcc.py` — Canonical solution. Scans float32 bit-planes of every tensor and detects `PCC{...}` payloads when packed as bytes.
- Exploratory/aux scripts that show the path we took:
  - `extract_flag.py`, `extract_flag2.py`, `extract_flag3.py`, `final_flag_search.py`, `deep_search.py` — early model/byte and ASCII probing.
  - `extract_specific_bytes.py`, `get_flag.py` — per-byte extraction from floats to surface printable characters; led to non-PCC-looking fragments.
  - `search_pcc.py`, `hunt_pcc_flag.py`, `decode_base64_streams.py`, `search_pcc_utf16.py`, `pattern_search_stride.py` — systematic PCC scanning across raw bytes, lanes, encodings (Base64/UTF‑16), strides, etc.
  - `scan_xor_pattern.py`, `xor_reveal_pcc.py` — XOR template and brute-force XOR scans looking for hidden PCC sequences.
  - `scan_PCC_any.py`, `search_transforms.py`, `pcc_bruteforce_from_fc1bias.py`, `reconstruct_message.py`, `find_pcc_from_byte0.py`, `extract_brace_substrings.py` — additional probes for brace substrings, Caesar shifts, transform-based reveals; helped confirm false paths.

The final, reliable extractor is `bitplane_hunt_pcc.py`.

---

## What we tried (chronological highlights)

1. Extract and inspect
   - Unpacked `ai-neurosteg.tar.gz` to get `model.pt`.
   - Basic PyTorch inspection showed standard layers: `fc1`, `fc2`, `fc3` with weights/biases.

2. Naive ASCII searches
   - Searched raw model bytes and per-tensor raw streams for ASCII, brace-delimited strings, and patterns like `flag{`, `ARENA{`, `CTF{}`.
   - Some bytes from `fc1.bias` produced readable punctuation and braces (e.g., `{ uc1gM ... }`) but not a valid `PCC{...}` flag. These were decoy-like fragments from directly sampling float bytes, not the actual hidden payload.

3. Transform/encoding hunts
   - Tried Base64/Base64url/Ascii85, UTF‑16 (LE/BE), stride-based interleaving, and XOR scans (template and brute force). No `PCC{...}` hits.

4. LSB/bit-lane attempts
   - Extracted least significant bits from per-byte lanes and tried packing into 8-bit and 7-bit ASCII; still no `PCC{...}`.

5. Bit-plane steganography (breakthrough)
   - Treat every float32 as a 32-bit word. For each tensor and bit position 0..31:
     - Take that single bit-plane across all elements.
     - Pack bits into bytes (tested LSB-first and MSB-first order).
     - Scan the resulting byte stream for `PCC{...}`.
   - Result: `fc2.weight`, bit 0 (LSB of the 32-bit word), packed into bytes with MSB-first, revealed the flag:
     - `PCC{st3g0_in_th3_w31ghts_OG}`

Why this works: float32 parameters use IEEE‑754 representation. Flipping/manipulating the least significant bits of the mantissa usually has negligible effect on model behavior, yet can embed an entire bitstream. The challenger hid the flag in the LSB bit-plane of `fc2.weight`.

---

## Reproduce locally

Already set up here with a venv at `.venv` and PyTorch/NumPy installed. To re-run the final extractor:

```powershell
# From the repo root (c:\Users\haryp\Downloads\ai)
C:/Users/haryp/Downloads/ai/.venv/Scripts/python.exe bitplane_hunt_pcc.py
```

Expected output (excerpt):

```
[fc2.weight] bit0 -> 8bit lsb_first=False: PCC{st3g0_in_th3_w31ghts_OG}
Found any: True
```

If you want a clean setup from scratch (optional):

```powershell
# Create and activate a venv (optional if .venv already exists)
python -m venv .venv
.\.venv\Scripts\pip.exe install --index-url https://download.pytorch.org/whl/cpu torch
.\.venv\Scripts\pip.exe install numpy
.\.venv\Scripts\python.exe bitplane_hunt_pcc.py
```

---

## Notes and lessons
- Don’t trust early ASCII-y results from raw float bytes; they can look meaningful without matching the expected flag format.
- A general stego playbook for ML parameters should include:
  - Raw file and per-tensor byte scans (LE/BE)
  - Per-byte lane concatenation
  - LSB extraction and packing (7‑bit/8‑bit, both bit orders)
  - Bit-plane scans across 0..31 (mantissa/exponent/sign)
  - Simple transforms (XOR, Base64, UTF‑16) as sanity checks
- The decisive technique here was single-bit-plane packing with MSB-first.

---

## Final answer
`PCC{st3g0_in_th3_w31ghts_OG}`
