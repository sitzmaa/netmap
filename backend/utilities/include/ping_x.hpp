#ifndef PING_X_HPP
#define PING_X_HPP

#include "commands.hpp"
#include <vector>
#include <string>

// Define bitwise flags for Ping options
#define PING_LATENCY     (1 << 0)  // Extract latency (RTT)
#define PING_PACKET_LOSS (1 << 1)  // Extract packet loss percentage
#define PING_TTL         (1 << 2)  // Extract TTL (Time to Live)
#define PING_IP          (1 << 3)  // Extract target IP address

class Ping{
public:
    static std::string run(const std::vector<std::string>& args);
};


#endif // PING_X_HPP