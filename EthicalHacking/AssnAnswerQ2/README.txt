ARP Scanner - arpscanner.py

Author: Joel Beh Zhirong

Description:
This program monitors live network traffic and detects ARP spoofing
attacks in real-time using Scapy's sniff() function. It maintains a
table of known IP-to-MAC address mappings and flags any conflicts
that may indicate an ARP spoofing attack.

Requirements:
- Python 3
- Scapy library (pip install scapy)
- Root/sudo privileges

How to run:
sudo python3 arpscanner.py

Expected output:
- Auto-detects the active network interface
- Displays new hosts as they are discovered
- Prints a warning if ARP spoofing is detected:
  [!!!] ARP SPOOFING DETECTED: <IP> changed from <old MAC> to <new MAC>

Notes:
- Must be run with sudo
- Runs independently of arpspoof.py
- Only captures ARP traffic to minimize network overhead
- Auto-identifies the active interface using Scapy's conf.iface