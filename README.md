# PCC CTF 2025 - Writeups Collection

![Profile Banner](https://img.shields.io/badge/Platform-AirOverflow%20Arena-purple?style=for-the-badge)
![Challenges Solved](https://img.shields.io/badge/Solved-9%2F9-green?style=for-the-badge)
![Points](https://img.shields.io/badge/Points-4150-blue?style=for-the-badge)

Complete writeups for all challenges solved during PCC CTF 2025 on the AirOverflow Arena platform.

## 🏆 Profile Overview

- **Username:** @harisisback
- **Level:** 11
- **Rank:** 18
- **Total Points:** 4150
- **Challenges Solved:** 9/32
- **Member Since:** October 18, 2025

## 📚 Challenge Writeups

### Web Exploitation

| Challenge | Difficulty | Points | Category | Status |
|-----------|-----------|--------|----------|--------|
| [DivaDaDida](./DivaDaDida/) | Hard | 600 | Web | ✅ Solved |
| [Mariooo](./mario/) | Medium | 350 | Web | ✅ Solved |
| [pexels](./pexels/) | Easy | 200 | Web | ✅ Solved |

### Forensics

| Challenge | Difficulty | Points | Category | Status |
|-----------|-----------|--------|----------|--------|
| [binded](./binded/) | Hard | 500 | Forensics | ✅ Solved |
| [tampered](./tampered/) | Medium | 400 | Forensics | ✅ Solved |

### Reverse Engineering

| Challenge | Difficulty | Points | Category | Status |
|-----------|-----------|--------|----------|--------|
| [AshfaqVM](./ashfaq/) | Expert | 700 | Reverse | ✅ Solved |
| [ROP](./rop/) | Medium | 400 | Reverse | ✅ Solved |

### AI/ML Security

| Challenge | Difficulty | Points | Category | Status |
|-----------|-----------|--------|----------|--------|
| [NeuroSteg](./neurosteg/) | Easy | 200 | AI | ✅ Solved |

### Miscellaneous

| Challenge | Difficulty | Points | Category | Status |
|-----------|-----------|--------|----------|--------|
| [inject](./inject/) | Easy | 200 | Misc | ✅ Solved |

## 🎯 Quick Summary

### [AshfaqVM](./ashfaq/) - Expert Reverse Engineering (700 pts)
Custom virtual machine challenge requiring VM emulation and byte-by-byte brute force attack to recover the flag. Built a complete Python VM emulator to trace execution and exploit per-character validation weakness.

**Key Techniques:** VM Emulation, Dynamic Analysis, Brute Force, Binary Instrumentation

---

### [binded](./binded/) - Hard Forensics (500 pts)
Network forensics challenge involving PCAP analysis and DES-CBC decryption. Extracted encrypted HTTP traffic, identified DES encryption with cookie-based key, and decrypted the session to reveal flag.

**Key Techniques:** PCAP Analysis, DES-CBC Decryption, Network Forensics, Cryptanalysis

---

### [DivaDaDida](./DivaDaDida/) - Hard Web (600 pts)
XSS challenge with character filtering requiring JSFuck-style payload construction. Bypassed character restrictions using only `c`, `i`, `o`, and `+` to build JavaScript payload that exfiltrated admin's cookie.

**Key Techniques:** XSS, Character Filter Bypass, JSFuck Encoding, Cookie Theft

---

### [inject](./inject/) - Easy Misc (200 pts)
Python code injection vulnerability in authentication system. The application used `exec()` with user-supplied password, allowing arbitrary code execution to read the flag file.

**Key Techniques:** Python Injection, `exec()` Exploitation, Command Injection

---

### [Mariooo](./mario/) - Medium Web (350 pts)
Spring Expression Language (SpEL) injection vulnerability. The application evaluated user input as Java expressions, allowing file system access through Java NIO classes to read the flag.

**Key Techniques:** SpEL Injection, Expression Language Exploitation, Java File I/O

---

### [NeuroSteg](./neurosteg/) - Easy AI (200 pts)
Steganography in PyTorch model weights. Flag was hidden in the LSB bit-plane of `fc2.weight` tensor parameters. Required bit-plane extraction and packing to reveal the embedded message.

**Key Techniques:** ML Steganography, Bit-plane Analysis, PyTorch Model Analysis

---

### [pexels](./pexels/) - Easy Web (200 pts)
JWT token forgery vulnerability. The application's token verification function didn't validate signatures, allowing privilege escalation by forging admin tokens to access restricted endpoints.

**Key Techniques:** JWT Forgery, Token Manipulation, Authentication Bypass

---

### [ROP](./rop/) - Medium Reverse (400 pts)
Return-Oriented Programming challenge with ROP chain analysis. Analyzed ROP gadgets to understand the flag decryption routine and extracted XOR keys from the chain to decode the flag.

**Key Techniques:** ROP Analysis, Binary Exploitation, XOR Decryption

---

### [tampered](./tampered/) - Medium Forensics (400 pts)
Ransomware forensics challenge involving XOR encryption with time-based key derivation. Analyzed bash history and file timestamps to reconstruct the base64-encoded time key and decrypt files.

**Key Techniques:** Ransomware Analysis, XOR Decryption, Filesystem Forensics, Base64 Encoding

---

## 🛠️ Tools & Technologies Used

- **Languages:** Python, JavaScript, PowerShell, Bash
- **Libraries:** PyTorch, Scapy, PyCryptodome, PyJWT, Requests
- **Tools:** Ghidra, Wireshark, 7-Zip, Git
- **Techniques:** VM Emulation, Network Analysis, Cryptanalysis, Web Exploitation, Binary Analysis

## 📁 Repository Structure

```
PCC-WRITEUPS/
├── README.md                 # This file
├── ashfaq/                   # AshfaqVM challenge
│   ├── README.md            # Detailed writeup
│   ├── full_vm.py           # VM emulator
│   ├── brute_decrypt.py     # Solution script
│   └── ...
├── binded/                   # binded challenge
│   ├── README.md            # Detailed writeup
│   ├── decrypt_des_sessions.py  # Solution script
│   └── ...
├── DivaDaDida/              # DivaDaDida challenge
│   ├── README.md            # Detailed writeup
│   ├── exploit.py           # Solution script
│   └── ...
├── inject/                   # inject challenge
│   ├── README.md            # Detailed writeup
│   ├── send_payload.ps1     # Solution script
│   └── ...
├── mario/                    # Mariooo challenge
│   ├── README.md            # Detailed writeup
│   ├── exploit.py           # Solution script
│   └── ...
├── neurosteg/               # NeuroSteg challenge
│   ├── README.md            # Detailed writeup
│   ├── bitplane_hunt_pcc.py # Solution script
│   └── ...
├── pexels/                   # pexels challenge
│   ├── README.md            # Detailed writeup
│   ├── solve_pexels.py      # Solution script
│   └── ...
├── rop/                      # ROP challenge
│   ├── README.md            # Detailed writeup
│   ├── decode_rop.py        # Solution script
│   └── ...
└── tampered/                 # tampered challenge
    ├── README.md            # Detailed writeup
    ├── final_decoder.py     # Solution script
    └── ...
```

## 🎓 Learning Outcomes

Throughout this CTF, I gained hands-on experience with:

1. **Advanced Reverse Engineering:** Built custom VM emulators and analyzed binary execution
2. **Network Forensics:** PCAP analysis and encrypted traffic decryption
3. **Web Security:** XSS, injection attacks, authentication bypass techniques
4. **Cryptanalysis:** DES, XOR, and custom encryption schemes
5. **AI/ML Security:** Steganography in neural network parameters
6. **Binary Exploitation:** ROP chain analysis and gadget identification

## 🔗 Links

- **Platform:** [AirOverflow Arena](https://arena.airoverflow.com/)
- **Profile:** [@harisisback](https://arena.airoverflow.com/profile/harisisback)
- **Repository:** [PCC-WRITEUPS](https://github.com/Perister2904/PCC-WRITEUPS)

## ⚠️ Disclaimer

These writeups are for educational purposes only. All challenges were solved on the official AirOverflow Arena CTF platform with proper authorization. Do not use these techniques on systems you do not own or have explicit permission to test.

## 📝 License

This repository is for educational purposes. Feel free to use these writeups for learning, but please provide attribution if you reference them.

## 🙏 Acknowledgments

- **AirOverflow Team** for creating engaging challenges
- **PCC CTF 2025** organizers
- The CTF community for sharing knowledge and techniques

---

**Last Updated:** October 24, 2025

**Status:** challenges solved (9/32) 
