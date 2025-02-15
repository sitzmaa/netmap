import sys
import socket
import argparse
from PyQt6.QtWidgets import (QApplication, QMainWindow, QPushButton, QLabel, 
                             QGraphicsScene, QGraphicsView, QGraphicsEllipseItem, QGraphicsTextItem, QVBoxLayout, QWidget, QLineEdit)
from PyQt6.QtCore import Qt, QThread, pyqtSignal
from PyQt6.QtGui import QBrush, QColor
import seed  # Assuming SEED library is installed

class NetworkNode(QGraphicsEllipseItem):
    def __init__(self, x, y, ip, details):
        super().__init__(0, 0, 50, 50)
        self.setBrush(QBrush(QColor(0, 0, 255)))
        self.setFlag(QGraphicsEllipseItem.GraphicsItemFlag.ItemIsSelectable)
        self.setPos(x, y)
        self.ip = ip
        self.details = details

        self.text = QGraphicsTextItem(ip, self)
        self.text.setDefaultTextColor(Qt.GlobalColor.white)
        self.text.setPos(10, 10)

    def mousePressEvent(self, event):
        print(f"Clicked on node: {self.ip}\nDetails: {self.details}")
        super().mousePressEvent(event)

class NetworkVisualizerApp(QMainWindow):
    def __init__(self, port):
        super().__init__()
        self.port = port
        self.setWindowTitle("NetMap - Network Visualizer")
        self.setGeometry(100, 100, 1000, 700)

        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        self.layout = QVBoxLayout(self.central_widget)

        self.label = QLabel("Network Map", self)
        self.layout.addWidget(self.label)

        # Graphics View for Network Map
        self.scene = QGraphicsScene()
        self.view = QGraphicsView(self.scene)
        self.layout.addWidget(self.view)

        # Command Input Box
        self.command_input = QLineEdit(self)
        self.command_input.setPlaceholderText("Enter command...")
        self.layout.addWidget(self.command_input)
        self.command_input.returnPressed.connect(self.send_command)

        # Buttons
        self.scan_btn = QPushButton("Scan Network", self)
        self.scan_btn.clicked.connect(self.scan_network)
        self.layout.addWidget(self.scan_btn)

        # Backend Thread
        self.backend_thread = BackendThread(self.port)
        self.backend_thread.new_data.connect(self.update_network)
        self.backend_thread.start()

        # SEED Network Visualization Setup
        self.network = seed.Network()

    def scan_network(self):
        self.backend_thread.send_command("scan")

    def send_command(self):
        command = self.command_input.text().strip()
        if command:
            self.backend_thread.send_command(command)
            self.command_input.clear()

    def update_network(self, data):
        self.scene.clear()
        nodes = data.split(";")
        x, y = 50, 50
        
        # Example static network data for visualization
        # Normally, this would come from your backend data
        example_nodes = [
            {"name": "Node 1", "type": "Router", "ip": "192.168.0.1", "details": "Main Router"},
            {"name": "Node 2", "type": "Device", "ip": "192.168.0.2", "details": "Device A"},
            {"name": "Node 3", "type": "Device", "ip": "192.168.0.3", "details": "Device B"}
        ]
        example_edges = [
            {"source": "Node 1", "destination": "Node 2"},
            {"source": "Node 1", "destination": "Node 3"}
        ]
        
        # Add nodes to SEED network (for visualization)
        for node_info in example_nodes:
            self.network.add_node(node_info['name'], **node_info)

        # Add edges to SEED network
        for edge in example_edges:
            self.network.add_edge(edge['source'], edge['destination'])
        
        # Visualization with SEED
        self.network.display()
        
        # Also adding graphical elements to PyQt6's QGraphicsView
        for node_info in example_nodes:
            ip, details = node_info['ip'], node_info['details']
            node = NetworkNode(x, y, ip, details)
            self.scene.addItem(node)
            x += 100
            if x > 800:
                x = 50
                y += 100

class BackendThread(QThread):
    new_data = pyqtSignal(str)

    def __init__(self, port):
        super().__init__()
        self.port = port
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.socket.connect(("localhost", self.port))
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