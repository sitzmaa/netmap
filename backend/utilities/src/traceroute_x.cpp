#include "traceroute_x.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <stdexcept>

std::string execute_traceroute(const std::string& target) {
    std::string command = "traceroute -n " + target + " 2>&1";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) throw std::runtime_error("Failed to execute traceroute command");

    char buffer[256];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

std::string parse_hop_count(const std::string& response) {
    std::istringstream stream(response);
    std::string line;
    int hop_count = 0;
    while (std::getline(stream, line)) hop_count++;
    return "Total Hops: " + std::to_string(hop_count) + "\n";
}

std::string parse_ips(std::string response) {
    std::regex re("\\b(\\d+\\.\\d+\\.\\d+\\.\\d+)\\b");
    std::smatch match;
    std::string result = "Traceroute IPs: ";
    bool found = false;
    while (std::regex_search(response, match, re)) {
        result += match[1].str() + " -> ";
        response = match.suffix().str();
        found = true;
    }
    return found ? result + "End\n" : "";
}

std::string traceroute_x(const std::string& target, int options) {
    std::string response = execute_traceroute(target);
    std::string result;

    if (options & TRACE_HOPS) result += parse_hop_count(response);
    if (options & TRACE_LATENCY) result += response;  // Return raw latency info
    if (options & TRACE_IPS) result += parse_ips(response);

    return result.empty() ? "No matching traceroute results\n" : result;
}

std::string Traceroute::run(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        return "Usage: traceroute_x <target> [options]\nOptions: --hops --latency --ips\n";
    }

    std::string target = args[1];
    int options = 0;
    
    for (size_t i = 2; i < args.size(); ++i) {
        if (args[i] == "--hops") options |= TRACE_HOPS;
        else if (args[i] == "--latency") options |= TRACE_LATENCY;
        else if (args[i] == "--ips") options |= TRACE_IPS;
    }

    return traceroute_x(target, options);
}