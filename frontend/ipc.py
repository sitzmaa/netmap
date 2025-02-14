import multiprocessing
import queue

class IPCManager:
    def __init__(self):
        self.queue = multiprocessing.Queue()
    
    def send_command(self, command):
        self.queue.put(command)
    
    def get_command(self):
        try:
            return self.queue.get_nowait()
        except queue.Empty:
            return None

ipc_manager = IPCManager()