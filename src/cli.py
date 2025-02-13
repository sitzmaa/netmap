import cmd

class NetworkCLI(cmd.Cmd):
    intro = "Welcome to the Network Parser CLI. Type help or ? to list commands."
    prompt = "(network-cli) "

    def do_scan(self, arg):
        """Scan a network for active hosts"""
        print("Scanning network...")

    def do_exit(self, arg):
        """Exit the CLI"""
        print("Exiting...")
        return True

def launch_cli():
    NetworkCLI().cmdloop()