import socket

HOST = "127.0.0.1"  # The server's hostname or IP address
PORT = 48595  # The port used by the server

while True:
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((HOST, PORT))
        command = input("Enter command: ")
        s.sendall(command.encode())
        data = s.recv(1024).decode()
        print(f"Received: {data}")
