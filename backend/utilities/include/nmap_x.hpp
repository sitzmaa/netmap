#ifndef NMAP_X_HPP
#define NMAP_X_HPP

#include "commands.hpp"
#include "network_state.hpp"
#include <vector>
#include <string>

// Define bitwise flags for Nmap options
#define NMAP_OPEN_PORT   (1 << 0)  // Extract open ports
#define NMAP_PUBLIC_IP   (1 << 1)  // Extract public IP
#define NMAP_MAC_ADDRESS (1 << 2)  // Extract MAC address
#define NMAP_DOMAIN      (1 << 3)  // Extract domain name
#define NMAP_OS          (1 << 4)  // Extract OS information

class Nmap{
public:
    static std::vector<ScanPayload> run(const std::vector<std::string>& args);
    static std::vector<ScanPayload> nmap_x(const std::string& target, uint8_t options);
};

#endif // NMAP_X_HPP