import sys
from PyQt6.QtWidgets import QApplication, QMainWindow, QLabel, QVBoxLayout, QWidget
import grpc_client  # Ensure this is your gRPC client module
import networkx as nx
import pyqtgraph as pg
from pyqtgraph.Qt import QtCore


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()

        self.setWindowTitle("Network Visualizer")
        self.setGeometry(100, 100, 800, 600)

        # Central widget and layout
        self.central_widget = QWidget()
        self.setCentralWidget(self.central_widget)
        self.layout = QVBoxLayout(self.central_widget)

        # Label for status
        self.label = QLabel("Waiting for network state...", self)
        self.layout.addWidget(self.label)

        # PyQtGraph widget for NetworkX graph
        self.graph_widget = pg.GraphicsLayoutWidget()
        self.layout.addWidget(self.graph_widget)

        # Create a NetworkX graph (will be updated later)
        self.network_graph = nx.Graph()
        self.plot = self.graph_widget.addPlot(title="Network Visualization")

        # Create a network visualization plot
        self.network_plot = self.plot.plot()

        self.show()

    def update_network_state(self, message, graph_data):
        """Update the GUI with network data and refresh the graph visualization."""
        self.label.setText(message)
        self.update_network_graph(graph_data)

    def update_network_graph(self, graph_data):
        """Update the NetworkX graph and render it on the PyQt6 window."""
        # Clear existing graph data
        self.network_graph.clear()

        # Add nodes and edges from the graph data
        for node in graph_data['nodes']:
            self.network_graph.add_node(node['name'], **node)
        for edge in graph_data['edges']:
            self.network_graph.add_edge(edge['source'], edge['destination'])

        # Create positions for nodes using spring layout
        pos = nx.spring_layout(self.network_graph)
        
        # Extract node positions and names for plotting
        node_positions = {node: (x * 100, y * 100) for node, (x, y) in pos.items()}
        
        # Prepare edge data for plotting
        edges = []
        for edge in self.network_graph.edges():
            node1, node2 = edge
            edges.append((node_positions[node1], node_positions[node2]))

        # Render the network graph
        self.plot.clear()
        for edge in edges:
            self.plot.plot([edge[0][0], edge[1][0]], [edge[0][1], edge[1][1]], pen='r')
        for node, (x, y) in node_positions.items():
            self.plot.plot([x], [y], symbol='o', symbolSize=10, symbolBrush='b')


def main():
    app = QApplication(sys.argv)
    window = MainWindow()

    graph_data = grpc_client.run()
    
    # Update the window with the fetched network data
    #window.update_network_state(message, graph_data)

    sys.exit(app.exec())


if __name__ == "__main__":
    main()