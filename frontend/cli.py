import socket
import argparse

class CLI:
    def __init__(self, port):
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect(("localhost", port))
        # Send 'False' to indicate that it's a CLI client
        self.socket.sendall(b"0\n")

    def run(self):
        while True:
            command = input("netmap> ")
            if command == "exit":
                self.socket.close()
                break
            self.send_command(command)

    def send_command(self, command):
        self.socket.sendall(command.encode() + b"\n")
        response = self.socket.recv(1024).decode()
        print(f"Server Response: {response}")

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="CLI client for NetMap")
    parser.add_argument("--port", type=int, default=12345, help="Specify the server port (default: 12345)")
    args = parser.parse_args()

    cli = CLI(args.port)
    cli.run()