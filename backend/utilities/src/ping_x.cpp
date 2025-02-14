#include "ping_x.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <stdexcept>

std::string execute_ping(const std::string& ip) {
    std::string command = "ping -c 4 " + ip + " 2>&1";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) throw std::runtime_error("Failed to execute ping command");

    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

std::string parse_ping_latency(const std::string& response) {
    std::regex re("min/avg/max/mdev = ([0-9.]+)/([0-9.]+)/([0-9.]+)/([0-9.]+)");
    std::smatch match;
    if (std::regex_search(response, match, re)) {
        return "Latency (RTT): " + match[2].str() + " ms\n";
    }
    return "";
}

std::string parse_ping_packet_loss(const std::string& response) {
    std::regex re("([0-9]+)% packet loss");
    std::smatch match;
    if (std::regex_search(response, match, re)) {
        return "Packet Loss: " + match[1].str() + "%\n";
    }
    return "";
}

std::string parse_ping_ttl(const std::string& response) {
    std::regex re("ttl=([0-9]+)");
    std::smatch match;
    if (std::regex_search(response, match, re)) {
        return "TTL: " + match[1].str() + "\n";
    }
    return "";
}

std::string ping_x(const std::string& ip, int options) {
    std::string response = execute_ping(ip);
    std::string result;

    if (options & PING_LATENCY) result += parse_ping_latency(response);
    if (options & PING_PACKET_LOSS) result += parse_ping_packet_loss(response);
    if (options & PING_TTL) result += parse_ping_ttl(response);
    if (options & PING_IP) result += "Target IP: " + ip + "\n";

    return result.empty() ? "No matching ping results\n" : result;
}

std::string Ping::run(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        return "Usage: ping_x <IP> [options]\nOptions: --latency --loss --ttl --ip\n";
    }

    std::string ip = args[1];
    int options = 0;
    
    for (size_t i = 2; i < args.size(); ++i) {
        if (args[i] == "--latency") options |= PING_LATENCY;
        else if (args[i] == "--loss") options |= PING_PACKET_LOSS;
        else if (args[i] == "--ttl") options |= PING_TTL;
        else if (args[i] == "--ip") options |= PING_IP;
    }
    
    return ping_x(ip, options);
}
