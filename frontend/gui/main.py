import sys
from PyQt6.QtWidgets import QApplication, QMainWindow, QLabel, QVBoxLayout, QWidget
import grpc_client

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Network Visualizer")
        self.setGeometry(100, 100, 800, 600)

        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        self.layout = QVBoxLayout(self.central_widget)

        self.label = QLabel("Waiting for network state...", self)
        self.layout.addWidget(self.label)

        self.show()

    def update_network_state(self, message):
        self.label.setText(message)

def main():
    app = QApplication(sys.argv)
    window = MainWindow()

    # Run the gRPC client to communicate with the Go server and update the UI
    grpc_client.run()

    sys.exit(app.exec())

if __name__ == "__main__":
    main()