#!/usr/bin/env python3
"""
q2.py -> Lamport's One-Time Password Scheme
Assignment 1, Question 2

Password : Alice1234567
n        : 5
Hash     : MD5

In Lamport's scheme:
  The server stores H^n(password) = H^5(password)
  Alice authenticates by sending passwords in REVERSE hash order
  Starting from H^(n-1) down to H^1

So the first 3 OTPs Alice transmits are:
  1st OTP: H^4(password)  <- Bob verifies: H(this) == H^5 stored
  2nd OTP: H^3(password)  <- Bob verifies: H(this) == H^4
  3rd OTP: H^2(password)  <- Bob verifies: H(this) == H^3
"""

import hashlib

def md5(value):
    # If input is a string, encode it to bytes first
    # If input is already bytes, use directly
    if isinstance(value, str):
        value = value.encode()
    return hashlib.md5(value).hexdigest()

#Starting password
password = "Alice1234567"

#Compute H^1 through H^5 by repeatedly hashing
print("Computing MD5 hashes:")
print(f"H^0 (original password): {password}")

h = password
for i in range(1, 6):
    h = md5(h)
    print(f"H^{i}: {h}")

print()

#H^5 is stored by Bob
h5 = md5(md5(md5(md5(md5(password)))))
print(f"Bob stores H^5: {h5}")
print()

#The first 3 OTPs Alice transmits are H^4, H^3, H^2
h4 = md5(md5(md5(md5(password))))
h3 = md5(md5(md5(password)))
h2 = md5(md5(password))

print("First 3 OTPs Alice transmits:")
print(f"1st OTP (H^4): {h4}")
print(f"2nd OTP (H^3): {h3}")
print(f"3rd OTP (H^2): {h2}")
