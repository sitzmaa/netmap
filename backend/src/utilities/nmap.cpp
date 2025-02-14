#include "nmap.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

bool scanHost(const std::string& target) {
    std::cout << "Scanning host: " << target << std::endl;

    // First, check if the host is reachable
    if (!pingHost(target)) {
        std::cerr << "Host " << target << " is not reachable." << std::endl;
        return false;
    }

    // Scan open ports
    auto openPorts = getOpenPorts(target);
    if (openPorts.empty()) {
        std::cout << "No open ports found on " << target << std::endl;
        return false;
    }

    std::cout << "Open ports for " << target << ":" << std::endl;
    for (const auto& port : openPorts) {
        std::cout << "Port " << port.first << ": " << port.second << std::endl;
    }
    return true;
}

bool scanHosts(const std::vector<std::string>& targets) {
    for (const auto& target : targets) {
        scanHost(target);
    }
    return true;
}

std::unordered_map<int, std::string> getOpenPorts(const std::string& target) {
    std::unordered_map<int, std::string> openPorts;
    // Implement port scanning logic here (simple example)
    for (int port = 20; port <= 1024; ++port) {
        int sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd == -1) {
            std::cerr << "Socket creation failed." << std::endl;
            continue;
        }

        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        addr.sin_addr.s_addr = inet_addr(target.c_str());

        int connection = connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));
        if (connection == 0) {
            openPorts[port] = "Open";
        }
        close(sockfd);
    }
    return openPorts;
}

bool pingHost(const std::string& target) {
    std::string cmd = "ping -c 1 " + target + " > /dev/null 2>&1";
    return system(cmd.c_str()) == 0;
}