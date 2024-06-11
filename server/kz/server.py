import json
import socket
import struct
import threading
import time

def handle_client(client_socket):
    try:
        while True:
            # 接收数据
            request = client_socket.recv(1024)
            if not request:
                break
            print(f"Received: {request.decode('utf-8')}")

            cmd = input("cmd >")
            msg = {
                "type":"cmd",
                "cmddata":cmd
            }

            msgbytes = json.dumps(msg).encode("utf-8")

            msglen = len(msgbytes)

            print(f"send len {msglen}")
            lenbytes = struct.pack('<I', msglen)


            client_socket.send(lenbytes)
            # 发送回显数据
            client_socket.send(msgbytes)
    finally:
        # 关闭连接
        client_socket.close()

def start_server(host, port):
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((host, port))
    server.listen(5)
    print(f"[*] Listening on {host}:{port}")

    while True:
        client_socket, addr = server.accept()
        print(f"[*] Accepted connection from {addr[0]}:{addr[1]}")

        # 使用线程处理客户端连接
        client_handler = threading.Thread(target=handle_client, args=(client_socket,))
        client_handler.start()

if __name__ == "__main__":
    HOST = "0.0.0.0"
    PORT = 1234
    start_server(HOST, PORT)
