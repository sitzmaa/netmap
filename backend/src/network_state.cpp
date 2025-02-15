#include "network_state.hpp"
#include <iostream>
#include <fstream>

void NetworkState::add_scan_result(const ScanPayload& payload) {
    if (nodes.find(payload.ip) == nodes.end()) {
        nodes[payload.ip] = {payload.ip, payload.mac_address, payload.domain, payload.os, payload.open_ports, {}};
    } else {
        Node& node = nodes[payload.ip];
        if (!payload.mac_address.empty()) node.mac_address = payload.mac_address;
        if (!payload.domain.empty()) node.domain = payload.domain;
        if (!payload.os.empty()) node.os = payload.os;
        if (!payload.open_ports.empty()) node.open_ports = payload.open_ports;
    }
}

void NetworkState::add_connection(const std::string& ip1, const std::string& ip2) {
    nodes[ip1].connections.push_back(ip2);
    nodes[ip2].connections.push_back(ip1);
}

void NetworkState::save_state() {
    json j;
    for (const auto& [ip, node] : nodes) {
        j[ip] = node.to_json();
    }
    std::ofstream file(state_file);
    if (file.is_open()) {
        file << j.dump(4);
        file.close();
    } else {
        std::cerr << "Failed to save network state." << std::endl;
    }
}

void NetworkState::load_state() {
    std::ifstream file(state_file);
    if (file.is_open()) {
        json j;
        file >> j;
        file.close();
        
        for (auto& [ip, data] : j.items()) {
            Node node;
            node.public_ip = data["public_ip"].get<std::string>();
            node.mac_address = data["mac_address"].get<std::string>();
            node.domain = data["domain"].get<std::string>();
            node.os = data["os"].get<std::string>();
            node.open_ports = data["open_ports"].get<std::vector<int>>();
            node.connections = data["connections"].get<std::vector<std::string>>();
            nodes[ip] = node;
        }
    }
}

void NetworkState::print_state() const {
    for (const auto& [ip, node] : nodes) {
        std::cout << "Node IP: " << ip << "\n";
        std::cout << "  MAC: " << node.mac_address << "\n";
        std::cout << "  Domain: " << node.domain << "\n";
        std::cout << "  OS: " << node.os << "\n";
        std::cout << "  Open Ports: ";
        for (int port : node.open_ports) std::cout << port << " ";
        std::cout << "\n  Connections: ";
        for (const auto& conn : node.connections) std::cout << conn << " ";
        std::cout << "\n\n";
    }
}