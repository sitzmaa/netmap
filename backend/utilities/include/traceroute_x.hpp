#ifndef TRACEROUTE_X_HPP
#define TRACEROUTE_X_HPP

#include <vector>
#include <string>

// Define bitwise flags for Traceroute options
#define TRACE_HOPS    (1 << 0)  // Extract number of hops
#define TRACE_LATENCY (1 << 1)  // Extract per-hop latency
#define TRACE_IPS     (1 << 2)  // Extract IP addresses in the route

class Traceroute{
public:
    static std::string run(const std::vector<std::string>& args);
};

#endif // TRACEROUTE_X_HPP