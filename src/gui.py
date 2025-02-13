from PyQt6.QtWidgets import QApplication, QMainWindow, QLabel, QVBoxLayout, QWidget
import sys

class NetworkGUI(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Network Parser GUI")
        self.setGeometry(100, 100, 800, 600)
        layout = QVBoxLayout()
        layout.addWidget(QLabel("Network Visualization Coming Soon!"))
        container = QWidget()
        container.setLayout(layout)
        self.setCentralWidget(container)

def launch_gui():
    app = QApplication(sys.argv)
    window = NetworkGUI()
    window.show()
    sys.exit(app.exec())