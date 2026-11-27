ARP Spoofer - arpspoof.py

Author: Joel Beh Zhirong

Description:
This program performs an ARP spoofing attack using the Scapy library.
It poisons the ARP tables of both the victim and the gateway, making
the attacker's machine act as a man-in-the-middle.

Requirements:
- Python 3
- Scapy library (pip install scapy)
- Root/sudo privileges

How to run:
sudo python3 arpspoof.py <Victim_IP> <Gateway_IP>

Example:
sudo python3 arpspoof.py 10.41.107.107 10.41.64.1

Expected output:
- Displays victim and gateway MAC addresses
- Continuously sends spoofed ARP packets
- Press Ctrl+C to stop and restore ARP tables

Notes:
- Must be run with sudo
- Victim machine is Metasploitable2
- Attacker machine is Kali Linux

