# Tampered CTF Challenge - Writeup

## Challenge Information
- **Name:** tampered
- **Category:** Forensics
- **Difficulty:** Medium
- **Points:** 400
- **Description:** Someone tampered with my AI research files! They used some kind of encryption. Can you recover the original content??

## FLAG
```
PCC{N3WB1E_L3V3L_R4NS0MW4R3_R3C0V3RY!!!}
```

**Translation:** NEWBIE LEVEL RANSOMWARE RECOVERY

---

## Solution Walkthrough

### Step 1: Initial Analysis

The challenge provides a RAR archive (`dist.rar`) containing:
- `tampered.tar` - A Linux root filesystem archive (120.7 MB)
- `integrity` - A file containing MD5 hash reference

Extract the archives:
```powershell
7z x dist.rar
7z x tampered.tar
```

### Step 2: Exploring the Filesystem

Navigate through the extracted Linux filesystem and discover key files:

**Location:** `/root/.bash_history`
```bash
cd /etc/research/txts
ls -la
cat b64
cat current
# key is -> current time in this format: 2025:10:11-HHMM --> b64
# attackers always leave keys in the system lol.... goodluck :D
# bye! - mikvirus
```

This reveals the critical clue about the encryption key!

**Location:** `/etc/research/txts/`
- 10 XOR-encrypted files: `*.txt.xor`
  - ai_education.txt.xor
  - ai_ethics.txt.xor
  - ai_healthcare.txt.xor
  - ai_research_2024.txt.xor
  - ai_robotics.txt.xor
  - ai_security.txt.xor
  - ai_transportation.txt.xor
  - automation_future.txt.xor (contains the flag!)
  - natural_language.txt.xor
  - neural_networks.txt.xor
- 2 empty files: `b64` and `current` (timestamps: 2025-10-11 20:11)
- XOR files all created: 2025-10-11 20:04:08

### Step 3: Understanding the Encryption

The bash_history clue states:
```
key is -> current time in this format: 2025:10:11-HHMM --> b64
```

**Key insight:** The arrow `-->` represents a PIPELINE operation!

This means:
1. Create timestamp string: `"2025:10:11-HHMM"`
2. **Base64 ENCODE** that string
3. Use the resulting base64 string as the XOR key

### Step 4: Decryption Process

#### Python Decryption Script

```python
import os
import base64
import re

# Path to the XOR files
xor_dir = r"/etc/research/txts"

# The key format from bash_history
# Try different times based on file timestamps
time_str = "2025:10:11-1504"  # The correct time!

# Step 1: Base64 encode the time string
key_b64 = base64.b64encode(time_str.encode())
# Result: b'MjAyNToxMDoxMS0xNTA0'

# Step 2: XOR decrypt the file
xor_file = os.path.join(xor_dir, "automation_future.txt.xor")

with open(xor_file, 'rb') as f:
    data = f.read()

# XOR with the base64-encoded key
decoded = bytearray()
for i, byte in enumerate(data):
    decoded.append(byte ^ key_b64[i % len(key_b64)])

decoded_str = decoded.decode('utf-8')

# Step 3: Extract the base64-encoded flag
# The decoded text contains: "Flag: UENDe04zV0IxRV9MM1YzTF9SNE5TME1XNFIzX1IzQzBWM1JZISEhfQ=="
b64_pattern = re.findall(r'[A-Za-z0-9+/]{30,}={0,2}', decoded_str)

for b64_str in b64_pattern:
    flag_bytes = base64.b64decode(b64_str)
    flag = flag_bytes.decode('utf-8')
    
    if 'PCC{' in flag:
        print(f"FLAG FOUND: {flag}")
        # Output: PCC{N3WB1E_L3V3L_R4NS0MW4R3_R3C0V3RY!!!}
```

### Step 5: Finding the Correct Time

The challenge requires brute-forcing the time component (HHMM). Since the clue mentions "current time", we need to find which timestamp was used:

**File timestamps:**
- XOR files created: `20:04:08` (8:04 PM)
- Empty marker files (b64, current): `20:11` (8:11 PM)

**The correct time: `15:04` (3:04 PM)**

This is likely the time when the ransomware was initially executed, before the files were encrypted at 20:04.

### Step 6: Complete Decryption

Full decryption process:
1. **Time string:** `2025:10:11-1504`
2. **Base64 encode:** `MjAyNToxMDoxMS0xNTA0`
3. **XOR decrypt** all files using this 20-character key
4. **Extract base64** from `automation_future.txt.xor`
5. **Base64 decode** the extracted string: `UENDe04zV0IxRV9MM1YzTF9SNE5TME1XNFIzX1IzQzBWM1JZISEhfQ==`
6. **Get flag:** `PCC{N3WB1E_L3V3L_R4NS0MW4R3_R3C0V3RY!!!}`

---

## Key Techniques Used

1. **Archive Extraction** - RAR → TAR → Linux filesystem
2. **Forensic Analysis** - Examining bash_history for clues
3. **Cryptanalysis** - Understanding XOR encryption with repeating key
4. **Base64 Encoding/Decoding** - Double encoding (for key generation and flag storage)
5. **Brute Force** - Testing all possible HHMM timestamps (00:00-23:59)
6. **Pattern Recognition** - Understanding the `-->` as a pipeline operation

---

## Tools Used

- **7-Zip** - Archive extraction (RAR/TAR)
- **Python 3** - Custom decryption scripts
- **Libraries:**
  - `base64` - Encoding/decoding
  - `re` - Regular expressions for pattern matching
  - `os` - File operations

---

## Lessons Learned

1. **Read clues carefully:** The `-->` arrow was crucial to understanding the key derivation
2. **File timestamps matter:** Ransomware forensics often relies on file metadata
3. **Double encoding is common:** Flag was XOR-encrypted, then base64-encoded
4. **Brute force has its place:** With only 1440 possible times (24h × 60m), brute forcing was efficient
5. **Leet speak in flags:** N3WB1E = NEWBIE, L3V3L = LEVEL, R4NS0MW4R3 = RANSOMWARE, R3C0V3RY = RECOVERY

---

## Challenge Creator Notes

This challenge simulates a realistic ransomware scenario where:
- Attacker (mikvirus) left behind clues
- Files were encrypted using time-based XOR encryption
- Forensic analysis of system files reveals the decryption method
- Recovery requires understanding both the encryption scheme and the key derivation

**Difficulty:** While categorized as "Medium", this challenge required:
- Understanding multi-stage encoding
- Brute-forcing timestamp values
- Recognizing patterns in partially corrupted data
- Persistence through multiple failed attempts

---

## Flag Submission

**Flag:** `PCC{N3WB1E_L3V3L_R4NS0MW4R3_R3C0V3RY!!!}`

**Points Earned:** 400

**Solve Rate:** 36% (as shown on platform)

---

*Challenge solved by analyzing filesystem artifacts, understanding encryption mechanisms, and systematically testing timestamp-based key derivation.*
