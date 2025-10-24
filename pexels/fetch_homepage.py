import requests
from urllib3.exceptions import InsecureRequestWarning
requests.packages.urllib3.disable_warnings(category=InsecureRequestWarning)
url = 'http://c1.arena.airoverflow.com:21204/'
try:
    r = requests.get(url, timeout=10)
    open('pwn-web-homepage.html','wb').write(r.content)
    print('Saved', 'pwn-web-homepage.html', 'status', r.status_code)
except Exception as e:
    print('Error:', e)
