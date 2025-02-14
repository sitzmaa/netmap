import argparse
import subprocess
import sys
import os
import time

DEFAULT_PORT = 12345

def open_terminal(command):
    """Open a new terminal window and run the command in the current directory."""
    cwd = os.getcwd()  # Get the current working directory

    if sys.platform == "darwin":  # macOS
        subprocess.Popen(["osascript", "-e", f'tell application "Terminal" to do script "cd {cwd} && {command}"'])
    elif sys.platform == "linux":  # Linux
        subprocess.Popen(["gnome-terminal", "--", "bash", "-c", f"cd {cwd} && {command}; exec bash"])
    elif sys.platform == "win32":  # Windows
        subprocess.Popen(["cmd", "/c", f'start cmd /k "cd /d {cwd} && {command}"'], shell=True)
    else:
        print(f"Unsupported OS: {sys.platform}")
        sys.exit(1)

def main():
    parser = argparse.ArgumentParser(description="Run the network backend, CLI, and GUI.")
    parser.add_argument("-P", "--port", type=int, default=DEFAULT_PORT, help="Specify the server port (default: 12345)")
    parser.add_argument("--no-gui", action="store_true", help="Do not start the GUI client")
    parser.add_argument("--no-cli", action="store_true", help="Do not start the CLI client")
    parser.add_argument("--server-only", action="store_true", help="Only start the server (no GUI, no CLI)")

    args = parser.parse_args()
    port = args.port

    # Start the server in the current working directory
    server_cmd = f"./backend/build/netmap_backend {port}"
    open_terminal(server_cmd)
    time.sleep(0.5)
    if not args.server_only:
        if not args.no_cli:
            cli_cmd = f"python3 ./frontend/cli.py --port {port}"
            open_terminal(cli_cmd)

        if not args.no_gui:
            gui_cmd = f"source venv/bin/activate && python3 ./frontend/gui.py --port {port}"
            open_terminal(gui_cmd)

if __name__ == "__main__":
    main()