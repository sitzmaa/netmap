from src.gui import launch_gui
from src.cli import launch_cli
import sys

if __name__ == "__main__":
    if len(sys.argv) > 1 and sys.argv[1] == "--cli":
        launch_cli()
    else:
        launch_gui()