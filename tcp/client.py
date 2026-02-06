from consts import *
import socket


def main():
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((SERVER_IP, SERVER_PORT))
    client.sendall(b"tes test t")


if __name__ == '__main__':
    main()
