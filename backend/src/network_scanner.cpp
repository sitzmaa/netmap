#include "network_scanner.hpp"
#include "network_state.hpp"
#include "nmap_x.hpp"
#include "traceroute_x.hpp"
#include <iostream>
#include <vector>

NetworkScanner::NetworkScanner(NetworkState& state) : network_state(state) {}

void NetworkScanner::scan_local_network() {
    std::string target = "192.168.1.0/24"; // Example local network range
    uint8_t options = NMAP_OPEN_PORT | NMAP_PUBLIC_IP | NMAP_MAC_ADDRESS | NMAP_DOMAIN | NMAP_OS;

    std::vector<ScanPayload> results = nmap_x(target, options);
    
    for (const auto& res : results) {
        network_state.add_scan_result(res); // Pass results to network_state
    }
    network_state.save_state(); // Save after scanning
}

void NetworkScanner::scan_target(const std::string& target) {
    uint8_t options = NMAP_OPEN_PORT | NMAP_PUBLIC_IP | NMAP_MAC_ADDRESS | NMAP_DOMAIN | NMAP_OS;
    std::vector<std::string> results = nmap_x(target, options);

    for (const auto& res : results) {
        std::cout << "Scan result: " << res << std::endl;
        network_state.add_scan_result(res);
    }
    network_state.save_state();
}

void NetworkScanner::traceroute_target(const std::string& target) {
    int options = TRACE_HOPS | TRACE_IPS;
    std::string result = traceroute_x(target, options);
    
    std::cout << "Traceroute result:\n" << result << std::endl;
    network_state.add_traceroute_result(target, result);
    network_state.save_state();
}
