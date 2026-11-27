#!/usr/bin/env python3
"""
connect.py -> Authentication Server (Two-Factor Authentication)

This program simulates the server that a user wants to connect to.
It handles two situations:

1. NEW USER REGISTRATION
    Prompts the user to set a password
    Checks the password meets basic security requirements
    Saves the username and password to passwords.txt

2. LOGIN WITH 2FA
    Checks the username exists in passwords.txt
    Checks the password matches what is stored
    Independently calculates the expected PIN and checks it matches
    Grants access only if ALL THREE checks pass

connect.py never talks to device.py.
It recalculates the PIN itself using the same formula.
Since both programs use the same username, password, and time window,
they will always produce the same PIN for the same 15-second window.

Usage:
  New user:  python3 connect.py <username> new
  Login:     python3 connect.py <username> <password> <pin>

Example:
  python3 connect.py Alice new
  python3 connect.py Alice MyPassword123 107283
"""

import sys       #read command line arguments and exit the program
import os        #check if passwords.txt exists before reading it
import hmac      #recalculate the expected PIN (must match device.py)
import hashlib   #provides the SHA256 algorithm used inside HMAC
import time      #used to determine the current 15-second time window

#The name of the file where usernames and passwords are stored
#Defined as a constant at the top so it is easy to change if needed
PASSWORDS_FILE = "passwords.txt"


# ------ PIN GENERATION (identical to device.py) ------


def generate_pin(username, password, time_window):
    """
    Generates a 6-digit PIN using HMAC-SHA256.
    This function is identical to the one in device.py.

    This is how 2FA works without communication:
    Both programs use the same formula, so they independently
    arrive at the same PIN for any given 15-second window.

    Parameters:
      username    - the user's username (e.g. "Alice")
      password    - the user's password (e.g. "MyPassword123")
      time_window - current Unix time divided by 15 (the 15-second block)
    """

    #Combine username and password into the secret key
    #This ensures different users get different PINs
    key = f"{username}:{password}".encode()

    #The time window is the message, this ties the PIN to a 15-second period
    message = str(time_window).encode()

    #HMAC-SHA256: a one-way function that mixes key and message securely
    #The result is a 64-character hexadecimal string
    digest = hmac.new(key, message, hashlib.sha256).hexdigest()

    #Convert the hex string to an integer and extract the last 6 digits
    pin = int(digest, 16) % 1000000

    #Always return exactly 6 digits, padding with leading zeros if necessary
    return f"{pin:06d}"


def is_pin_valid(username, password, submitted_pin):
    """
    Checks whether the PIN submitted by the user is currently valid.

    We check TWO time windows:
        The CURRENT window (the PIN currently shown on Device)
        The PREVIOUS window (in case the user grabbed the PIN just before it changed)

    This gives the user a small grace period and prevents frustrating
    login failures caused by slight timing differences.

    Returns True if the PIN matches either window, False otherwise.
    """

    #Get the current time window
    current_window = int(time.time()) // 15

    #Check both the current and previous time window
    for window in [current_window, current_window - 1]:
        expected_pin = generate_pin(username, password, window)

        #If the submitted PIN matches the expected PIN for this window, it's valid
        if expected_pin == submitted_pin:
            return True

    #PIN did not match either window -> it is invalid or expired
    return False



# ------ PASSWORD VALIDATION ------


def validate_password(password):
    """
    Checks that a password meets basic security requirements.

    Requirements:
        At least 8 characters long
        Contains at least one uppercase letter (A-Z)
        Contains at least one lowercase letter (a-z)
        Contains at least one digit (0-9)

    Returns a tuple: (True, "OK") if valid
                        (False, "reason") if invalid
    """

    #Check minimum length
    if len(password) < 8:
        return False, "Password must be at least 8 characters long."

    #any() returns True if at least one character satisfies the condition
    #c.isupper() checks if character c is an uppercase letter
    if not any(c.isupper() for c in password):
        return False, "Password must contain at least one uppercase letter."

    #c.islower() checks if character c is a lowercase letter
    if not any(c.islower() for c in password):
        return False, "Password must contain at least one lowercase letter."

    #c.isdigit() checks if character c is a number (0-9)
    if not any(c.isdigit() for c in password):
        return False, "Password must contain at least one digit."

    #All checks passed
    return True, "OK"



# ------ PASSWORDS FILE OPERATIONS ------

def load_users():
    """
    Reads passwords.txt and loads all registered users into a dictionary.

    The file format is one user per line:
        username:password
    Example:
        Alice:MyPassword123
        Boris:SecurePass1

    Returns a dictionary where:
        key   = username (e.g. "Alice")
        value = password (e.g. "MyPassword123")

    Returns an empty dictionary if the file does not exist yet.
    """

    users = {}  #Start with an empty dictionary

    #os.path.exists() checks if the file is present before trying to open it
    #Without this check, the program would crash if no one has registered yet
    if not os.path.exists(PASSWORDS_FILE):
        return users  # Return empty dictionary - no users registered yet

    #Open the file in read mode ("r")
    with open(PASSWORDS_FILE, "r") as f:
        for line in f:
            line = line.strip()  #Remove newline characters at end of line

            #Only process lines that contain a colon (valid format)
            if ":" in line:
                #Split on the first colon only, in case password contains colons
                parts = line.split(":", 1)
                username = parts[0]
                password = parts[1]
                users[username] = password  #Store in dictionary

    return users


def save_user(username, password):
    """
    Appends a new user to passwords.txt.

    Opens the file in append mode ("a") so existing users are not overwritten.
    If the file does not exist yet, Python creates it automatically.

    Format written: username:password
    """

    #"a" mode = append -> adds to the end of the file
    with open(PASSWORDS_FILE, "a") as f:
        f.write(f"{username}:{password}\n")  #\n starts a new line after each entry



# ------ NEW USER REGISTRATION ------


def register_new_user(username):
    """
    Handles the registration of a new user.

    Steps:
        check the username is not already taken
        prompt for a password and ask them to confirm it
        validate the password meets security requirements
        save the username and password to passwords.txt
    """

    #Load existing users to check if username is already taken
    users = load_users()

    if username in users:
        print(f"Error: Username '{username}' is already registered.")
        sys.exit(1)  #Stop the program -> cannot register the same username twice

    #Keep asking for a password until a valid one is provided
    while True:
        #input() displays a prompt and waits for the user to type something
        password1 = input("Enter new password: ")
        password2 = input("Confirm new password: ")

        #Check both entries match - catches typos during registration
        if password1 != password2:
            print("Passwords do not match. Please try again.\n")
            continue  # Go back to the top of the loop and ask again

        #Check the password meets our security requirements
        valid, message = validate_password(password1)
        if not valid:
            print(f"Invalid password: {message}")
            print("Please try again.\n")
            continue  # Go back to the top of the loop and ask again

        #If we reach here, the password is valid and both entries match
        break

    #Save the new user to passwords.txt
    save_user(username, password1)
    print(f"User '{username}' registered successfully.")



# ------ LOGIN WITH 2FA ------


def login(username, password, pin):
    """
    Verifies a login attempt using two-factor authentication.

    Three checks must ALL pass for access to be granted:
        Username must exist in passwords.txt
        Password must match what is stored for that username
        PIN must match the expected PIN for the current time window

    If any check fails, access is denied with a clear message.
    """

    #Load all registered users from passwords.txt
    users = load_users()

    #Check 1: Does this username exist?
    if username not in users:
        print("Authentication failed: username not found.")
        sys.exit(1)  # Stop the program - do not proceed further

    #Check 2: Does the password match?
    if users[username] != password:
        print("Authentication failed: incorrect password.")
        sys.exit(1)  # Stop the program - do not proceed further

    #Check 3: Is the PIN valid for the current time window?
    #Connect independently recalculates what the PIN should be and compares it to what the user submitted
    if not is_pin_valid(username, password, pin):
        print("Authentication failed: invalid or expired PIN.")
        sys.exit(1)  # Stop the program - do not proceed further

    #All three checks passed -> grant access
    print(f"Access granted. Welcome, {username}!")



# ------ MAIN PROGRAM - reads arguments and decides what to do ------


def main():
    """
    Reads the command line arguments and runs either:
      - register_new_user() if called with "new"
      - login() if called with a password and PIN
    """

    #sys.argv contains everything typed in the terminal as a list:
    #  sys.argv[0] = "connect.py"   (always the program name)
    #  sys.argv[1] = username
    #  sys.argv[2] = "new" OR password
    #  sys.argv[3] = pin (only for login)

    #New user registration: Connect username new
    if len(sys.argv) == 3 and sys.argv[2] == "new":
        username = sys.argv[1]
        register_new_user(username)

    #Login: Connect username password pin
    elif len(sys.argv) == 4:
        username = sys.argv[1]
        password = sys.argv[2]
        pin      = sys.argv[3]
        login(username, password, pin)

    #Anything else is an error -> show correct usage
    else:
        print("Usage:")
        print("  New user: python3 connect.py <username> new")
        print("  Login:    python3 connect.py <username> <password> <pin>")
        sys.exit(1)


#Only run main() if this file is being executed directly
if __name__ == "__main__":
    main()
