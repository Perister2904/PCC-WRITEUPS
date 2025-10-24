from scapy.all import *
import binascii

# Read the pcap file
packets = rdpcap('challenge.pcap')

print(f"Total packets: {len(packets)}")
print("\n" + "="*80)

# Analyze packet types
protocols = {}
for pkt in packets:
    if pkt.haslayer(TCP):
        protocols['TCP'] = protocols.get('TCP', 0) + 1
    if pkt.haslayer(UDP):
        protocols['UDP'] = protocols.get('UDP', 0) + 1
    if pkt.haslayer(ICMP):
        protocols['ICMP'] = protocols.get('ICMP', 0) + 1
    if pkt.haslayer(DNS):
        protocols['DNS'] = protocols.get('DNS', 0) + 1

print("\nProtocol distribution:")
for proto, count in protocols.items():
    print(f"  {proto}: {count}")

print("\n" + "="*80)

# Look for HTTP traffic
http_data = []
for pkt in packets:
    if pkt.haslayer(TCP) and pkt.haslayer(Raw):
        payload = pkt[Raw].load
        if b'HTTP' in payload or b'GET' in payload or b'POST' in payload:
            http_data.append(payload)

if http_data:
    print("\nHTTP Traffic found:")
    for i, data in enumerate(http_data[:10]):  # Show first 10
        print(f"\n--- HTTP Packet {i+1} ---")
        try:
            print(data.decode('utf-8', errors='ignore')[:500])
        except:
            print(data[:500])

print("\n" + "="*80)

# Look for TCP streams and extract payloads
tcp_streams = {}
for pkt in packets:
    if pkt.haslayer(TCP) and pkt.haslayer(Raw):
        src = pkt[IP].src if pkt.haslayer(IP) else "unknown"
        dst = pkt[IP].dst if pkt.haslayer(IP) else "unknown"
        sport = pkt[TCP].sport
        dport = pkt[TCP].dport
        stream_key = f"{src}:{sport} -> {dst}:{dport}"
        
        if stream_key not in tcp_streams:
            tcp_streams[stream_key] = []
        tcp_streams[stream_key].append(pkt[Raw].load)

print(f"\nTCP streams found: {len(tcp_streams)}")
for stream, payloads in list(tcp_streams.items())[:5]:
    print(f"\nStream: {stream}")
    combined = b''.join(payloads)
    print(f"  Total bytes: {len(combined)}")
    print(f"  Preview: {combined[:200]}")

print("\n" + "="*80)

# Look for suspicious strings or flags
print("\nSearching for flag patterns...")
flag_patterns = [b'flag', b'FLAG', b'CTF', b'ARENA', b'arena']

for pkt in packets:
    if pkt.haslayer(Raw):
        payload = pkt[Raw].load
        for pattern in flag_patterns:
            if pattern in payload:
                print(f"\nFound '{pattern.decode()}' in packet:")
                print(f"  {payload[:500]}")
                break
