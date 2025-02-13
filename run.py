import multiprocessing
import argparse
from src.gui import launch_gui
from src.cli import launch_cli

def run_cli():
    launch_cli()

def run_gui():
    launch_gui()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Run the Network Parser Tool")
    parser.add_argument("-g", "--gui", action="store_true", help="Run GUI only")
    parser.add_argument("-c", "--cli", action="store_true", help="Run CLI only")
    
    args = parser.parse_args()

    if args.gui:
        run_gui()
    elif args.cli:
        run_cli()
    else:
        # Default: Run both GUI and CLI in separate processes
        cli_process = multiprocessing.Process(target=run_cli)
        gui_process = multiprocessing.Process(target=run_gui)

        cli_process.start()
        gui_process.start()

        cli_process.join()
        gui_process.join()