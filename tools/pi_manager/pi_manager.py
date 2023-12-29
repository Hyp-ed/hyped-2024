import hashlib
from os import execv, fork
import socket
import json

HYPED_POD = "/home/infernox/work/hyped-2024/build/bin/hyped_pod"
CONFIG = "/home/infernox/work/hyped-2024/config/pod.toml"
HOST = 'localhost'
PORT = 48595

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    while True:
        s.listen()
        conn, addr = s.accept()
        with conn:
            data = conn.recv(1024)
            if data == b'get_hashes':
                hashes = {"result": "success"}
                hashes['hyped_pod'] = hashlib.md5(
                    open(HYPED_POD, 'rb').read()).hexdigest()
                hashes['config'] = hashlib.md5(
                    open(CONFIG, 'rb').read()).hexdigest()
                reply = json.dumps(hashes)
            elif data == b'start':
                if fork() == 0:
                    execv(HYPED_POD, [HYPED_POD, CONFIG])
                reply = json.dumps({"result": "success"})
            else:
                reply = json.dumps(
                    {"result": "error", "message": "Unknown command"})
            conn.sendall(reply.encode())
