#ifndef NMAP_H
#define NMAP_H

#include <string>
#include <vector>
#include <unordered_map>

bool scanHost(const std::string& target);
bool scanHosts(const std::vector<std::string>& targets);
std::unordered_map<int, std::string> getOpenPorts(const std::string& target);
bool pingHost(const std::string& target);

#endif // NMAP_H