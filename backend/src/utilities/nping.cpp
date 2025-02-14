#include "nping.hpp"
#include "nmap.hpp"
#include <iostream>

void advancedPing(const std::string& target) {
    std::cout << "Performing advanced ping on " << target << std::endl;

    // First, ping the target
    if (pingHost(target)) {
        std::cout << target << " is alive!" << std::endl;
    } else {
        std::cout << target << " is not reachable!" << std::endl;
    }

    // Then, check open ports
    auto openPorts = getOpenPorts(target);
    if (!openPorts.empty()) {
        std::cout << "Open ports on " << target << ":" << std::endl;
        for (const auto& port : openPorts) {
            std::cout << "Port " << port.first << ": " << port.second << std::endl;
        }
    } else {
        std::cout << "No open ports found on " << target << std::endl;
    }
}