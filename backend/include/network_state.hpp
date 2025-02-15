#ifndef NETWORK_STATE_HPP
#define NETWORK_STATE_HPP

#include <iostream>
#include <fstream>
#include <nlohmann/json.hpp>
#include <unordered_map>
#include <vector>

using json = nlohmann::json;

struct ScanPayload {
    std::string ip;
    std::string mac_address;
    std::string domain;
    std::string os;
    std::vector<int> open_ports;
    std::vector<std::string> connections;
};

struct Node {
    std::string public_ip;
    std::string mac_address;
    std::string domain;
    std::string os;
    std::vector<int> open_ports;
    std::vector<std::string> connections;

    json to_json() const {
        return {
            {"public_ip", public_ip},
            {"mac_address", mac_address},
            {"domain", domain},
            {"os", os},
            {"open_ports", open_ports},
            {"connections", connections}
        };
    }
};

class NetworkState {
private:
    std::unordered_map<std::string, Node> nodes;
    std::string state_file = "network_state.json";

public:
    void add_scan_result(const ScanPayload& payload);
    void add_connection(const std::string& ip1, const std::string& ip2);
    void save_state();
    void load_state();
    void print_state() const;
};

#endif // NETWORK_STATE_HPP
