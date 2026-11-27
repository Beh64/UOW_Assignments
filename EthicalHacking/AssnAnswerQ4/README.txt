Ransomware - ransom.py

Author: Joel Beh Zhirong

Description:
This program simulates a ransomware attack using Python and OpenSSL.
It encrypts a victim's file using AES-128-CBC symmetric encryption,
encrypts the symmetric key using RSA public key encryption, deletes
the original files, displays a ransom message, and then decrypts
everything back to recover the original file.

Requirements:
- Python 3
- OpenSSL installed (default on Kali)
- A file named my_secrets.txt in the same directory

How to run:
Step 1 Create the victim's file:
echo "This is my secret data" > my_secrets.txt

Step 2 Run the ransomware:
python3 ransom.py

Expected output:
- Author and date printed
- RSA keypair generated (private.pem, public.pem)
- my_secrets.txt encrypted to data_cipher.txt
- key.txt encrypted to key_cipher.txt (base64)
- key.txt and my_secrets.txt deleted
- Ransom message displayed
- data_cipher.txt decrypted back to my_secrets.txt

Files generated:
- data_cipher.txt: AES encrypted version of my_secrets.txt
- key_cipher.txt: RSA encrypted symmetric key (base64)
- private.pem: attacker's RSA private key
- public.pem: attacker's RSA public key

Notes:
- subprocess module is used to call OpenSSL commands
- All ciphertext outputs are in base64 format (human readable)
- Decryption simulates the attacker receiving payment and
  sending back the decryption key