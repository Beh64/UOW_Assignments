Reverse Shell - server.py & revshell.py

Author: Joel Beh Zhirong

Description:
server.py is the attacker's server that waits for a connection from 
the victim and allows the attacker to run Linux commands remotely 
on the victim's machine. revshell.py is the reverse shell malware 
that runs on the victim's machine and connects back to the server.
JSON is used to transfer data between server and client.

Requirements:
- Python 3
- Both programs run on the same Kali machine for testing

How to run:
Step 1 - Start the server first (Terminal 1):
python3 server.py

Step 2 - Run the reverse shell (Terminal 2):
python3 revshell.py

Step 3 - Type Linux commands in Terminal 1 at the shell> prompt
         including the cd command to change directories.

Notes:
- server.py listens on port 4444
- revshell.py connects back to the server automatically
- The cd command is supported and persists across commands
- Type "exit" to close the connection