#ifndef NETWORK_STATE_H
#define NETWORK_STATE_H

#include "node.h"
#include <unordered_map>
#include <vector>

class NetworkState {
private:
    std::unordered_map<std::string, Node> nodes;  // Store nodes by public IP
    std::unordered_map<std::string, std::vector<std::string>> connections; // Store edges

public:

    NetworkState() = default;

    ~NetworkState() = default;

    void addNode(const Node& node);
    void addConnection(const std::string& ip1, const std::string& ip2);
    Node* getNode(const std::string& public_ip);
    std::vector<std::string> getConnections(const std::string& public_ip);

    std::string toJSON() const; // Convert state to JSON for storage
    void saveToFile(const std::string& filename);
    void loadFromFile(const std::string& filename);

};

#endif // NETWORK_STATE_H