from consts import *
import socket
import threading


class ClientThread(threading.Thread):
    def __init__(self, id, ip, port, s):
        threading.Thread.__init__(self)
        self.id = id
        self.ip = ip
        self.port = port
        self.socket = s
        self.stop_event = threading.Event()

        print(f"Worker #{self.id} connected from: {self.ip}:{self.port}")

    def run(self):
        try:
            while not self.stop_event.is_set():
                data = self.socket.recv(BUF_SIZE)

                if not data:
                    break

                print(data.decode())

        # Connection died or socket closed
        except (ConnectionResetError, OSError):
            print(f"Worker #{self.id} exploded ! ! !")

        finally:
            self.terminate()

    def terminate(self):
        if not self.stop_event.is_set():
            print(f"Worker #{self.id} terminating . . .")
            self.stop_event.set()
            # Forces blocking recv() call to stop
            self.socket.shutdown(socket.SHUT_RDWR)
            self.socket.close()


class Server:
    def __init__(self):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.socket.bind((SERVER_IP, SERVER_PORT))
        self.socket.listen(1)
        self.socket.settimeout(1.0)
        self.threads = []
        self.latest_id = 0

        print(f"Server listening on: {SERVER_IP}:{SERVER_PORT}")

    def terminate(self):
        print("Server terminating . . .")

        # Finish all work before joining
        print("Killing workers . . .")
        for t in self.threads:
            t.terminate()

        print("Joining threads . . .")
        for t in self.threads:
            t.join()

        self.socket.close()
        print("Server terminated!")

    def manage_workers(self):
        try:
            while True:
                try:
                    s, (ip, port) = self.socket.accept()
                except socket.timeout:
                    continue

                # Prunes dead threads
                self.threads = [t for t in self.threads if t.is_alive()]
                self.latest_id += 1

                new_worker = ClientThread(self.latest_id, ip, port, s)
                new_worker.start()
                self.threads.append(new_worker)
        except KeyboardInterrupt:
            self.terminate()


def main():
    server = Server()
    server.manage_workers()


if __name__ == '__main__':
    main()
