#ifndef NODE_H
#define NODE_H

#include <string>
#include <nlohmann/json.hpp>

class Node {
public:
    // Default constructor
    Node();

    // Parameterized constructor
    Node(const std::string& pub_ip, const std::string& loc_ip, const std::string& dom, const std::string& mac);

    // Getters
    std::string getPublicIP() const;
    std::string getLocalIP() const;
    std::string getDomain() const;
    std::string getMacAddress() const;

    // Setters
    void setPublicIP(const std::string& pub_ip);
    void setLocalIP(const std::string& loc_ip);
    void setDomain(const std::string& dom);
    void setMacAddress(const std::string& mac);

    // Serialization methods
    nlohmann::json to_json() const;
    static Node from_json(const nlohmann::json& j);

    // Debugging
    void print() const;

private:
    std::string public_ip;
    std::string local_ip;
    std::string domain;
    std::string mac_address;
};

#endif // NODE_H