# netmap

A Network Visualization Tool in Python

## Overview

netmap is a Python-based tool designed to help you visualize network structures by generating interactive network maps. It works with commands like traceroute, dig, and ping to create a visual representation of the nodes, their relationships, and details such as IPs and domains.

## Goals

The main objective of netmap is to provide an easy-to-use network visualization tool that can analyze both local and public networks. It can display network topologies, trace routes, resolve DNS, and potentially integrate with packet capture tools like Wireshark.

## Features

### Core Features:
•	Network Map Visualization: Display the network as an interactive map, with nodes representing devices or servers and edges representing connections.

•	Traceroute Integration: Visualize the path packets take across the network with information about intermediate nodes.

•	Ping Highlighting: Easily highlight nodes based on ping responses for quick diagnostics.

•	DNS Resolution: Show hostnames alongside IP addresses for easier identification of nodes.

### Upcoming Features:
#### Local Network Visualization
Visualize devices on your local network, displaying details like:
	•	Local IP addresses
	•	DNS names (if available)
 
#### Public Network Visualization
Create a network map for scanned public gateways using:
	•	DNS and IP information
	•	Traceroute paths
 
#### Wireshark Integration
Plan to integrate with Wireshark for the ability to:
	•	Highlight packet capture data
	•	Provide visual graphs based on Wireshark capture analysis

## Installation
1.	Clone the repository:
 ``` bash
git clone https://github.com/yourusername/netmap.git
cd netmap
 ```
2.	Create a virtual environment (optional but recommended):
  ``` bash
  python3 -m venv venv
  source venv/bin/activate   # For Linux/macOS
  venv\Scripts\activate      # For Windows
  ```
3.	Install the required dependencies:
  ``` bash
  pip install -r requirements.txt
  ```

## Usage

## Running the Visualizer:

To launch the graphical user interface (GUI) and start visualizing networks:
``` bash
python3 run.py
```

## Contributing

Contributions to the project are welcome! If you have any ideas or want to contribute, follow these steps:
	1.	Fork the repository
	2.	Create a new branch for your feature or bug fix
	3.	Commit your changes and push them to your fork
	4.	Submit a pull request

Make sure your code follows the PEP 8 style guide and that all tests pass.

## License

This project is licensed under the GNU General Public License v3.0 - see the LICENSE file for details.







  
