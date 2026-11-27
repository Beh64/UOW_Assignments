"""
q2.py - Assignment 2, Question 2
SYN Flood attack: connection table fill rate and bandwidth requirements.

Given:
  - SYN connection table holds 512 slots
  - Each half-open connection occupies a slot for 6 * 30 = 180 seconds
  - Each TCP SYN packet is 64 bytes = 512 bits
"""


# Q2.1  Attack request rate

TABLE_SIZE        = 512          # number of slots
TIMEOUT_SECONDS   = 6 * 30      # 180 seconds per slot (6 ticks of 30 s)
TIMEOUT_MINUTES   = TIMEOUT_SECONDS / 60   # 3 minutes

# To keep the table continuously full the attacker must send one new request
# every time an entry expires.  Rate = TABLE_SIZE / lifetime
rate_per_minute = TABLE_SIZE / TIMEOUT_MINUTES
rate_per_second = TABLE_SIZE / TIMEOUT_SECONDS

print("=" * 50)
print("Q2.1 – Attack rate to keep table full")
print("=" * 50)
print(f"Table size        : {TABLE_SIZE} slots")
print(f"Slot lifetime     : {TIMEOUT_SECONDS} s  ({TIMEOUT_MINUTES} min)")
print(f"Rate              : {TABLE_SIZE} / {TIMEOUT_MINUTES} min")
print(f"                  = {rate_per_minute:.4f} requests / minute")
print(f"                  ≈ {round(rate_per_minute)} requests / minute")
print(f"  (= {rate_per_second:.6f} requests / second)")


# Q2.2  Bandwidth consumed

PACKET_BYTES = 64
PACKET_BITS  = PACKET_BYTES * 8   # 512 bits

bandwidth_bps  = rate_per_second * PACKET_BITS
bandwidth_kbps = bandwidth_bps / 1000

print()
print("=" * 50)
print("Q2.2 – Bandwidth consumed by the attacker")
print("=" * 50)
print(f"Packet size       : {PACKET_BYTES} bytes = {PACKET_BITS} bits")
print(f"Attack rate       : {rate_per_second:.6f} req/s")
print(f"Bandwidth         : {rate_per_second:.6f} * {PACKET_BITS}")
print(f"                  = {bandwidth_bps:.4f} bits/second")
print(f"                  ≈ {bandwidth_kbps:.2f} kbps")

# Exact fraction form for clarity
import fractions
exact_bps = fractions.Fraction(TABLE_SIZE * PACKET_BITS, TIMEOUT_SECONDS)
print(f"\nExact (fraction)  : {TABLE_SIZE * PACKET_BITS}/{TIMEOUT_SECONDS} "
      f"= {exact_bps} bps = {float(exact_bps):.4f} bps")
