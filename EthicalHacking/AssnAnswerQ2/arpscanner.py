#!/usr/bin/env python3
import sys
from scapy.all import sniff, ARP, conf
from datetime import datetime

author = "Joel Beh Zhirong"
current_date = datetime.now().strftime("%d/%m/%Y")
print(f"Author: {author}")
print(f"Date: {current_date}")

ip_mac_table = {}

def process_packet(pkt):
    if pkt.haslayer(ARP) and pkt[ARP].op == 2:  # only look at replies
        ip, mac = pkt[ARP].psrc, pkt[ARP].hwsrc
        if ip in ip_mac_table and ip_mac_table[ip] != mac:
            print(f"[!] ARP SPOOFING DETECTED: {ip} changed from {ip_mac_table[ip]} to {mac}")
        else:
            print(f"[*] {ip} -> {mac}")
        ip_mac_table[ip] = mac

def main():
    iface = conf.iface  # scapy auto-detects the default active interface
    print(f"[*] Monitoring on {iface} (ARP only)")
    sniff(iface=iface, filter="arp", prn=process_packet, store=False)

if __name__ == "__main__":
    main()
