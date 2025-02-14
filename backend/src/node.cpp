#include "node.h"
#include <iostream>

// Default constructor
Node::Node() = default;

// Parameterized constructor
Node::Node(const std::string& pub_ip, const std::string& loc_ip, const std::string& dom, const std::string& mac)
    : public_ip(pub_ip), local_ip(loc_ip), domain(dom), mac_address(mac) {}

// Getters
std::string Node::getPublicIP() const { return public_ip; }
std::string Node::getLocalIP() const { return local_ip; }
std::string Node::getDomain() const { return domain; }
std::string Node::getMacAddress() const { return mac_address; }

// Setters
void Node::setPublicIP(const std::string& pub_ip) { public_ip = pub_ip; }
void Node::setLocalIP(const std::string& loc_ip) { local_ip = loc_ip; }
void Node::setDomain(const std::string& dom) { domain = dom; }
void Node::setMacAddress(const std::string& mac) { mac_address = mac; }

// Convert Node to JSON
nlohmann::json Node::to_json() const {
    return {
        {"public_ip", public_ip},
        {"local_ip", local_ip},
        {"domain", domain},
        {"mac_address", mac_address}
    };
}

// Convert JSON to Node
Node Node::from_json(const nlohmann::json& j) {
    return Node(
        j.at("public_ip").get<std::string>(),
        j.at("local_ip").get<std::string>(),
        j.at("domain").get<std::string>(),
        j.at("mac_address").get<std::string>()
    );
}

// Print node details (for debugging)
void Node::print() const {
    std::cout << "Public IP: " << public_ip << "\n"
              << "Local IP: " << local_ip << "\n"
              << "Domain: " << domain << "\n"
              << "MAC Address: " << mac_address << std::endl;
}