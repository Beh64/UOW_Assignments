#!/usr/bin/env python3
"""
device.py - The PIN Generator (Two-Factor Authentication)

This program simulates a hardware authentication device (like Google Authenticator).
It generates a new 6-digit one-time PIN every 15 seconds.

The PIN is generated using the username, password, and current time window.
This means:
  - Different users get different PINs
  - The PIN changes every 15 seconds
  - connect.py can verify the PIN without communicating with device.py

Usage: python3 device.py <username> <password>
Example: python3 device.py Alice MyPassword123
"""

import sys       # Used to read command line arguments (username, password)
import hmac      # Used to generate a secure keyed hash (the PIN)
import hashlib   # Provides the SHA256 algorithm used inside HMAC
import time      # Used to get the current time and create 15-second windows



# ------ PIN GENERATION ------


def generate_pin(username, password, time_window):
    """
    Generates a 6-digit PIN using HMAC-SHA256.

    How it works:
      1. Combine username and password as the SECRET KEY
         e.g. "Alice:MyPassword123"

      2. Use the TIME WINDOW as the message
         The time window is: floor(current Unix time / 15)
         This means the number only changes every 15 seconds
         e.g. if Unix time is 1714000020, window = 1714000020 // 15 = 114266668

      3. Run HMAC-SHA256 with the key and message
         This produces a long 64-character hex string
         e.g. "a3f8c2d1e4b5...f0a1"

      4. Convert the hex string to a large integer, then take
         the last 6 digits using modulo 1,000,000
         e.g. 987654321 % 1000000 = 654321

      5. Format as 6 digits with leading zeros if needed
         e.g. 654321 -> "654321", 123 -> "000123"

    Why HMAC and not just MD5?
      HMAC mixes the secret key INTO the hash in a cryptographically
      secure way. This means knowing the PIN cannot reveal the password.
    """

    #Step 1: Create the secret key by combining username and password
    #.encode() converts the string to bytes, which HMAC requires
    key = f"{username}:{password}".encode()

    #Step 2: Use the time window as the message
    #This ties the PIN to a specific 15-second period
    message = str(time_window).encode()

    #Step 3: Run HMAC-SHA256
    #hmac.new(key, message, algorithm) creates the HMAC object
    #.hexdigest() returns the result as a 64-character hex string
    digest = hmac.new(key, message, hashlib.sha256).hexdigest()

    #Step 4: Convert hex string to integer, then extract last 6 digits
    #int(digest, 16) converts from base-16 (hex) to a normal integer
    #% 1000000 keeps only the last 6 digits
    pin = int(digest, 16) % 1000000

    #Step 5: Format as exactly 6 digits, padding with leading zeros if needed
    #f"{pin:06d}" means: integer, minimum 6 digits wide, pad with zeros
    return f"{pin:06d}"



# ------ MAIN PROGRAM ------


def main():
    #Check that the user provided exactly 2 arguments (username and password)
    #sys.argv is a list of everything typed in the terminal:
    #   sys.argv[0] = "device.py"  (program name, always present)
    #   sys.argv[1] = username
    #   sys.argv[2] = password
    if len(sys.argv) != 3:
        print("Usage: python3 device.py <username> <password>")
        print("Example: python3 device.py Alice MyPassword123")
        sys.exit(1)  #Exit with error code 1 (means something went wrong)

    #Read the username and password from the command line
    username = sys.argv[1]
    password = sys.argv[2]

    print(f"Device started for user: {username}")
    print("A new PIN is generated every 15 seconds.")
    print("Use the displayed PIN in connect.py to log in.")
    print("Press Ctrl-C to stop.\n")

    #Keep track of the last time window we generated a PIN for
    #Start as None so we always generate a PIN immediately on startup
    last_window = None

    #Keep running forever until the user presses Ctrl-C
    try:
        while True:
            #Calculate the current time window
            #time.time() gives current Unix time (seconds since Jan 1 1970)
            #Dividing by 15 and dropping the remainder groups time into
            #15-second blocks - everyone in the same block gets the same number
            current_window = int(time.time()) // 15

            #Only generate a new PIN when the time window has changed
            if current_window != last_window:
                pin = generate_pin(username, password, current_window)
                print(f"Device: {pin}")

                #Remember this window so we don't print the same PIN again
                last_window = current_window

            #Wait 1 second before checking the clock again
            #This avoids the program running at full CPU speed unnecessarily
            time.sleep(1)

    except KeyboardInterrupt:
        #This runs when the user presses Ctrl-C
        print("\nDevice stopped.")


#This ensures main() only runs when this file is executed directly,
#not if it were imported by another Python file
if __name__ == "__main__":
    main()
