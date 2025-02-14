import subprocess

def run_traceroute(target):
    return subprocess.run(["./backend/bin/traceroute", target], capture_output=True, text=True).stdout

def run_ping(target):
    return subprocess.run(["./backend/bin/ping", target], capture_output=True, text=True).stdout


def run_dns_lookup(target):
    return subprocess.run(["./backend/bin/dns_lookup", target], capture_output=True, text=True).stdout


def run_network_scan():
    return subprocess.run(["./backend/bin/network_scan"], capture_output=True, text=True).stdout