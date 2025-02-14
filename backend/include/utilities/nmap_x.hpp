#ifndef NMAP_X_HPP
#define NMAP_X_HPP

#include <string>
#include <vector>
#include <cstdint>

// Define bitwise flags for Nmap options
constexpr uint8_t NMAP_OPEN_PORT   = 0x01;
constexpr uint8_t NMAP_PUBLIC_IP   = 0x02;
constexpr uint8_t NMAP_MAC_ADDRESS = 0x04;
constexpr uint8_t NMAP_DOMAIN      = 0x08;
constexpr uint8_t NMAP_OS          = 0x10;

class NmapX {
public:
    // Main function to execute the scan and parse the result
    static std::vector<std::string> scan(const std::string& target, uint8_t options);

private:
    // Helper function to run the nmap scan and return the XML output
    static std::string run_nmap(const std::string& target, uint8_t options);
    
    // Function to parse the nmap XML result and extract requested data
    static std::vector<std::string> parse_nmap_xml(const std::string& xml_data, uint8_t options);
};

#endif // NMAP_X_HPP
