#include "network_state.h"
#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;


// Add a node to the network
void NetworkState::addNode(const Node& node) {
    if (nodes.find(node.getPublicIP()) == nodes.end()) {
        nodes[node.getPublicIP()] = node;
    } else {
        std::cerr << "Node with IP " << node.getPublicIP() << " already exists.\n";
    }
}

// Add a connection between two nodes
void NetworkState::addConnection(const std::string& ip1, const std::string& ip2) {
    if (nodes.find(ip1) == nodes.end() || nodes.find(ip2) == nodes.end()) {
        std::cerr << "Error: One or both nodes not found.\n";
        return;
    }
    connections[ip1].push_back(ip2);
    connections[ip2].push_back(ip1);
}

// Retrieve a node by public IP
Node* NetworkState::getNode(const std::string& public_ip) {
    if (nodes.find(public_ip) != nodes.end()) {
        return &nodes[public_ip];
    }
    return nullptr;
}

// Get connections for a given node
std::vector<std::string> NetworkState::getConnections(const std::string& public_ip) {
    if (connections.find(public_ip) != connections.end()) {
        return connections[public_ip];
    }
    return {};
}

// Convert NetworkState to JSON format
std::string NetworkState::toJSON() const {
    json json_data;
    for (const auto& [ip, node] : nodes) {
        json_data["nodes"][ip] = {
            {"public_ip", node.getPublicIP()},
            {"local_ip", node.getLocalIP()},
            {"domain", node.getDomain()},
            {"mac_address", node.getMacAddress()},
        };
    }

    for (const auto& [ip, conn_list] : connections) {
        json_data["connections"][ip] = conn_list;
    }

    return json_data.dump(4); // Pretty print with 4-space indentation
}

// Save network state to a JSON file
void NetworkState::saveToFile(const std::string& filename) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << "\n";
        return;
    }
    file << toJSON();
    file.close();
}

// Load network state from a JSON file
void NetworkState::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file for reading: " << filename << "\n";
        return;
    }

    json json_data;
    file >> json_data;
    file.close();

    nodes.clear();
    connections.clear();

    // Load nodes
    for (const auto& [ip, node_data] : json_data["nodes"].items()) {
        nodes[ip] = Node(
            node_data["public_ip"],
            node_data["local_ip"],
            node_data["domain"],
            node_data["mac_address"]
        );
    }

    // Load connections
    for (const auto& [ip, conn_list] : json_data["connections"].items()) {
        connections[ip] = conn_list.get<std::vector<std::string>>();
    }
}