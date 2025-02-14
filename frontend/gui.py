import sys
import socket
import argparse
from PyQt6.QtWidgets import QApplication, QMainWindow, QPushButton, QLabel
from PyQt6.QtCore import QThread, pyqtSignal

class NetworkVisualizerApp(QMainWindow):
    def __init__(self, port):
        super().__init__()
        self.port = port
        self.setWindowTitle("NetMap - Network Visualizer")
        self.setGeometry(100, 100, 800, 600)

        self.label = QLabel("Network Map", self)
        self.label.move(350, 50)

        self.btn = QPushButton("Scan Network", self)
        self.btn.move(350, 100)
        self.btn.clicked.connect(self.scan_network)

        self.backend_thread = BackendThread(self.port)
        self.backend_thread.new_data.connect(self.update_label)
        self.backend_thread.start()

    def scan_network(self):
        self.backend_thread.send_command("ping")

    def update_label(self, data):
        self.label.setText(f"Server Response: {data}")

class BackendThread(QThread):
    new_data = pyqtSignal(str)

    def __init__(self, port):
        super().__init__()
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect(("localhost", self.port))
        # Send 'True' to indicate that it's a GUI client
        self.socket.sendall(b"1\n")
    
    def run(self):
        while True:
            data = self.socket.recv(1024).decode()
            if data:
                self.new_data.emit(data)
    
    def send_command(self, command):
        self.socket.sendall(command.encode() + b"\n")

def run():
    parser = argparse.ArgumentParser(description="GUI client for NetMap")
    parser.add_argument("--port", type=int, default=12345, help="Specify the server port (default: 12345)")
    args = parser.parse_args()

    app = QApplication(sys.argv)
    window = NetworkVisualizerApp(args.port)
    window.show()
    sys.exit(app.exec())

if __name__ == "__main__":
    run()