import socket
import json
import base64
from datetime import datetime

author = "Joel Beh Zhirong"
current_date = datetime.now().strftime("%d/%m/%Y")
print(f"Author: {author}")
print(f"Date: {current_date}")

def listen_on(ip, port):
    global target

    listener = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    listener.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
    listener.bind((ip, port))
    listener.listen(0)
    print("Listening...")
    target, address = listener.accept()
    print(f"Connection from {address} established.")

def send(data):
    json_data = json.dumps(data)
    target.send(json_data.encode('utf-8'))

def receive():
    json_data = ''
    while True:
        try:
            json_data += target.recv(1024).decode('utf-8')
            return json.loads(json_data)
        except ValueError:
            continue

def server_run():
    while True:
        try:
            command = input("shell> ")

            # Only allow harmless test commands
            allowed_commands = ["pwd", "ls", "whoami"]

            if command == "exit":
                send({"cmd": "exit"})
                response = receive()
                print(response["output"])
                break

            if command not in allowed_commands:
                print("Command not allowed for this test.")
                continue

            send({"cmd": command})

            response = receive()
            print(response["output"])

        except Exception as e:
            print(f"Connection error: {e}")
            break

listen_on('10.41.97.146', 4444) #Replace 10.0.2.15 with your Kali IP
server_run()

