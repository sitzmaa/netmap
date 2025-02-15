from PyQt6.QtWidgets import QLabel

class UIManager:
    def __init__(self, window):
        self.window = window

    def update_network_state(self, state_data):
        # Handle the data and update the UI elements accordingly
        self.window.label.setText(f"Network state: {state_data}")