#include "nmap_x.hpp"
#include <iostream>
#include <cstdlib>
#include <memory>
#include <array>
#include <stdexcept>
#include <sstream>
#include <tinyxml2.h>

using namespace tinyxml2;

// Helper function to run the nmap scan and return the XML output
std::string run_nmap(const std::string& target, int options) {
    std::ostringstream cmd_stream;
    cmd_stream << "nmap -oX - "; // Use standard output in XML format

    // Add options to the command based on bitwise flags
    if (options & NMAP_OPEN_PORT) {
        cmd_stream << "-p- "; // Scan all ports
    }
    if (options & NMAP_PUBLIC_IP) {
        cmd_stream << "--unprivileged "; // Optional: run nmap without root privileges
    }
    if (options & NMAP_MAC_ADDRESS) {
        cmd_stream << "--osscan-guess "; // Guess OS based on MAC address
    }
    if (options & NMAP_DOMAIN) {
        cmd_stream << "--dns-servers "; // Optionally specify DNS server
    }
    if (options & NMAP_OS) {
        cmd_stream << "-O "; // Enable OS detection
    }

    cmd_stream << target;

    std::string cmd = cmd_stream.str();
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), fclose);

    if (!pipe) {
        throw std::runtime_error("Failed to run nmap command");
    }

    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

// Function to parse the nmap XML result and extract requested data
std::vector<std::string> parse_nmap_xml(const std::string& xml_data, int options) {
    XMLDocument doc;
    std::vector<std::string> results;

    // Parse the XML data
    if (doc.Parse(xml_data.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("Failed to parse XML data from nmap");
    }

    XMLElement* root = doc.RootElement();
    if (root == nullptr) {
        throw std::runtime_error("Failed to find root element in nmap XML");
    }

    // Extract the requested information based on bitwise flags
    if (options & NMAP_OPEN_PORT) {
        XMLElement* host = root->FirstChildElement("host");
        while (host) {
            XMLElement* ports = host->FirstChildElement("ports");
            if (ports) {
                for (XMLElement* port = ports->FirstChildElement("port"); port; port = port->NextSiblingElement("port")) {
                    const char* port_str = port->Attribute("portid");
                    if (port_str) {
                        results.push_back("Open Port: " + std::string(port_str));
                    }
                }
            }
            host = host->NextSiblingElement("host");
        }
    }
    if (options & NMAP_PUBLIC_IP) {
        XMLElement* host = root->FirstChildElement("host");
        while (host) {
            XMLElement* address = host->FirstChildElement("address");
            if (address) {
                const char* ip = address->Attribute("addr");
                if (ip) {
                    results.push_back("Public IP: " + std::string(ip));
                }
            }
            host = host->NextSiblingElement("host");
        }
    }
    if (options & NMAP_MAC_ADDRESS) {
        XMLElement* host = root->FirstChildElement("host");
        while (host) {
            XMLElement* hostnames = host->FirstChildElement("hostnames");
            if (hostnames) {
                for (XMLElement* hostname = hostnames->FirstChildElement("hostname"); hostname; hostname = hostname->NextSiblingElement("hostname")) {
                    const char* mac = hostname->Attribute("mac");
                    if (mac) {
                        results.push_back("MAC Address: " + std::string(mac));
                    }
                }
            }
            host = host->NextSiblingElement("host");
        }
    }
    if (options & NMAP_DOMAIN) {
        XMLElement* host = root->FirstChildElement("host");
        while (host) {
            XMLElement* hostnames = host->FirstChildElement("hostnames");
            if (hostnames) {
                for (XMLElement* hostname = hostnames->FirstChildElement("hostname"); hostname; hostname = hostname->NextSiblingElement("hostname")) {
                    const char* domain = hostname->Attribute("name");
                    if (domain) {
                        results.push_back("Domain: " + std::string(domain));
                    }
                }
            }
            host = host->NextSiblingElement("host");
        }
    }
    if (options & NMAP_OS) {
        XMLElement* host = root->FirstChildElement("host");
        while (host) {
            XMLElement* os = host->FirstChildElement("os-fingerprint");
            if (os) {
                const char* os_name = os->Attribute("os");
                if (os_name) {
                    results.push_back("OS: " + std::string(os_name));
                }
            }
            host = host->NextSiblingElement("host");
        }
    }

    return results;
}

// Main function to execute the scan and parse the result
std::vector<std::string> nmap_x(const std::string& target, int options) {
    try {
        std::string scan_result = run_nmap(target, options);
        return parse_nmap_xml(scan_result, options);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return {}; // Return empty list on error
    }
}
