#ifndef TRACEROUTE_X_HPP
#define TRACEROUTE_X_HPP

#include <string>

// Define bitwise flags for Traceroute options
#define TRACE_HOPS    (1 << 0)  // Extract number of hops
#define TRACE_LATENCY (1 << 1)  // Extract per-hop latency
#define TRACE_IPS     (1 << 2)  // Extract IP addresses in the route

std::string traceroute_x(const std::string& target, int options);

#endif // TRACEROUTE_X_HPP