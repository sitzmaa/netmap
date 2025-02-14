#ifndef PING_X_HPP
#define PING_X_HPP

#include <string>

// Define bitwise flags for Ping options
#define PING_LATENCY     (1 << 0)  // Extract latency (RTT)
#define PING_PACKET_LOSS (1 << 1)  // Extract packet loss percentage
#define PING_TTL         (1 << 2)  // Extract TTL (Time to Live)
#define PING_IP          (1 << 3)  // Extract target IP address

std::string ping_x(const std::string& ip, int options);

#endif // PING_X_HPP