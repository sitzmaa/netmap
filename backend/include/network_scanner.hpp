#ifndef NETWORK_SCANNER_HPP
#define NETWORK_SCANNER_HPP

#include <string>
#include <vector>
#include "network_state.hpp"

class NetworkScanner {
public:
    NetworkScanner();
    
    // Scans the local network using nmap and updates network state
    void scanNetwork(NetworkState& state);
    
};

#endif // NETWORK_SCANNER_HPP
