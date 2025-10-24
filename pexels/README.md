# Pexels CTF Solution

Clean solution repository for the **pexels** CTF challenge from Arena/AirOverflow platform.

## Challenge Information

- **Name:** pexels
- **Category:** Web Exploitation
- **Difficulty:** Easy (E4)
- **Points:** 200
- **Solve Rate:** 46%
- **Platform:** Arena by AirOverflow

## Vulnerability Overview

The challenge involves a "Pexels Gallery API" web application with the following exploitable vulnerabilities:

1. **JWT Token Forgery** - The main vulnerability. The application's `verify_token()` function doesn't properly validate JWT signatures, allowing attackers to forge tokens with elevated privileges.
2. **SQL Injection** (alternate approach) - The login endpoint may be vulnerable to SQL injection attacks.
3. **Endpoint Enumeration** - Various API endpoints can be discovered and exploited.

## Solution Files

### Core Exploit Scripts

- **`solve_pexels.py`** - Main solution using JWT token forgery attack
  - Logs in as guest user
  - Decodes JWT without verification
  - Forges new token with admin role
  - Accesses `/api/admin` endpoint to retrieve flag

- **`exploit_pexels.py`** - Comprehensive exploit toolkit
  - Tests SQL injection payloads
  - Enumerates API endpoints
  - Tests common credentials
  - Attempts blind SQL injection
  - Performs UNION-based data extraction

- **`fetch_homepage.py`** - Utility to download the challenge homepage for analysis

## Quick Start

### Prerequisites

- Python 3.8+
- Internet connection to reach the challenge instance

### Setup

```powershell
# Clone or navigate to this directory
cd pexels-solution

# Create virtual environment (recommended)
python -m venv .venv
.\.venv\Scripts\Activate.ps1

# Install dependencies
pip install -r requirements.txt
```

### Running the Solution

**Method 1: JWT Forgery (Primary Solution)**

```powershell
# Update TARGET URL in solve_pexels.py if needed
python solve_pexels.py
```

Expected output:
```
[*] Step 1: Login as guest to get a valid token structure
[+] Login response: 200
[+] Token received: eyJ...
[*] Step 2: Decode token to see payload
[+] Original payload: {'username': 'guest', 'role': 'guest', ...}
[*] Step 3: Modify role to 'admin' (no verification needed!)
[+] Forged token: eyJ...
[*] Step 4: Access /api/admin with forged token
[+] Response status: 200
[+] Response: {"flag": "ARENA{...}"}
======================================================================
[!] FLAG FOUND: ARENA{...}
======================================================================
```

**Method 2: Comprehensive Attack (Alternate)**

```powershell
# Update TARGET URL in exploit_pexels.py if needed
python exploit_pexels.py
```

This will test multiple attack vectors and display results.

## Technical Details

### JWT Forgery Attack Flow

1. **Initial Login**: Authenticate with guest credentials (`guest:guest123`)
2. **Token Analysis**: Decode the JWT without signature verification
3. **Privilege Escalation**: Modify the `role` claim from `guest` to `admin`
4. **Token Forging**: Re-encode the JWT with any secret (signature not verified)
5. **Flag Retrieval**: Access the `/api/admin` endpoint with forged token

### Key Code Weakness

The application's token verification is broken:

```python
def verify_token(token):
    # This function doesn't actually verify anything!
    pass  # No signature verification
```

This allows attackers to create valid tokens without knowing the secret key.

## Configuration

Before running, update the `TARGET` variable in each script to match your challenge instance:

```python
# solve_pexels.py
TARGET = "http://c1.arena.airoverflow.com:58585"

# exploit_pexels.py  
TARGET = "http://c1.arena.airoverflow.com:21204"
```

## Dependencies

See `requirements.txt`:
- `requests` - HTTP library for API interaction
- `PyJWT` - JWT encoding/decoding
- `urllib3` - HTTP client (dependency of requests)

## Troubleshooting

**Connection Refused / Timeout**
- Ensure the challenge container is started on the platform
- Verify the TARGET URL matches your instance
- Check firewall/network settings

**Module Not Found**
```powershell
pip install -r requirements.txt
```

**Wrong Flag Format**
- Ensure you're using the correct challenge instance URL
- The flag format is `ARENA{...}` or `PCC{...}`

## Files Included

```
pexels-solution/
├── README.md              # This file
├── requirements.txt       # Python dependencies
├── solve_pexels.py       # Primary JWT forgery exploit
├── exploit_pexels.py     # Comprehensive attack toolkit
└── fetch_homepage.py     # Homepage fetching utility
```

## Additional Notes

- The solution demonstrates the importance of proper JWT signature verification
- Always validate JWT signatures server-side before trusting token claims
- This is an educational CTF challenge - do not use these techniques on systems you don't own

## Author

Solution created for the pexels challenge on Arena by AirOverflow platform.

---

**Flag Format:** `ARENA{...}` or `PCC{...}`

**Challenge Status:** ✅ Solved
