from datetime import datetime
import subprocess
import os

author = "Joel Beh Zhirong"
current_date = datetime.now().strftime("%d/%m/%Y")
print(f"Author: {author}")
print(f"Date: {current_date}")

# A shell command like:
# openssl rand -base64 16 > key.txt
# becomes:
result = subprocess.run(
    ["openssl", "rand", "-base64", "16"],
    capture_output=True,  # captures stdout
    text=True,            # returns output as string not bytes
    check=True            # raises error if command fails
)
# then save to file manually:
with open("key.txt", "w") as f:
    f.write(result.stdout)
    
subprocess.run(["openssl", "genrsa", "-out", "private.pem", "2048"], check=True)
subprocess.run(["openssl", "rsa", "-in", "private.pem", "-pubout", "-out", "public.pem"], check=True)
  
# openssl enc -aes-128-cbc -salt -base64 -in my_secrets.txt -out data_cipher.txt -pass file:key.txt
subprocess.run(
    ["openssl", "enc", "-aes-128-cbc", "-salt", "-base64",
     "-in", "my_secrets.txt", "-out", "data_cipher.txt", "-pass", "file:key.txt"],
    check=True
)

subprocess.run(["openssl", "pkeyutl", "-encrypt", "-pubin", "-inkey", "public.pem", "-in", "key.txt", "-out", "key_cipher_raw.txt"], check=True)
subprocess.run(["openssl", "base64", "-in", "key_cipher_raw.txt", "-out", "key_cipher.txt"], check=True)

os.remove("key.txt")
os.remove("my_secrets.txt")

print("Your file important.txt is encrypted. To decrypt it, you need to pay me $10,000 and send key_cipher.txt to me.")

subprocess.run(["openssl", "base64", "-d", "-in", "key_cipher.txt", "-out", "key_cipher_raw.txt"], check=True)
subprocess.run(["openssl", "pkeyutl", "-decrypt", "-inkey", "private.pem", "-in", "key_cipher_raw.txt", "-out", "key.txt"], check=True)
subprocess.run(["openssl", "enc", "-d", "-aes-128-cbc", "-base64", "-in", "data_cipher.txt", "-out", "my_secrets.txt", "-pass", "file:key.txt"], check=True)

