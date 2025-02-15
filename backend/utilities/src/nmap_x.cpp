#include "nmap_x.hpp"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <tinyxml2.h>

using namespace tinyxml2;

std::string run_nmap(const std::string& target, uint8_t options) {
    std::ostringstream cmd_stream;
    cmd_stream << "nmap -oX - ";

    if (options & NMAP_OPEN_PORT) cmd_stream << "-p- ";
    if (options & NMAP_PUBLIC_IP) cmd_stream << "--unprivileged ";
    if (options & NMAP_MAC_ADDRESS) cmd_stream << "--osscan-guess ";
    if (options & NMAP_DOMAIN) cmd_stream << "--dns-servers ";
    if (options & NMAP_OS) cmd_stream << "-O ";

    cmd_stream << target;
    std::string cmd = cmd_stream.str();

    FILE* pipe = popen(cmd.c_str(), "r");
    if (!pipe) throw std::runtime_error("Failed to run nmap command");

    char buffer[128];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

#include "network_state.hpp"
#include <tinyxml2.h>
#include <vector>
#include <string>

using namespace tinyxml2;

std::vector<ScanPayload> parse_nmap_xml(const std::string& xml_data, uint8_t options) {
    XMLDocument doc;
    std::vector<ScanPayload> results;

    if (doc.Parse(xml_data.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("Failed to parse XML data from nmap");
    }

    XMLElement* root = doc.RootElement();
    if (!root) {
        throw std::runtime_error("Failed to find root element in nmap XML");
    }

    XMLElement* host = root->FirstChildElement("host");
    while (host) {
        ScanPayload payload;
        bool has_data = false;

        if (options & NMAP_OPEN_PORT) {
            XMLElement* ports = host->FirstChildElement("ports");
            if (ports) {
                for (XMLElement* port = ports->FirstChildElement("port"); port; port = port->NextSiblingElement("port")) {
                    const char* port_str = port->Attribute("portid");
                    if (port_str) {
                        payload.open_ports.push_back(std::stoi(port_str)); // Store open port as int
                        has_data = true;
                    }
                }
            }
        }
        if (options & NMAP_PUBLIC_IP) {
            XMLElement* address = host->FirstChildElement("address");
            if (address) {
                const char* ip = address->Attribute("addr");
                if (ip) {
                    payload.ip = ip;
                    has_data = true;
                }
            }
        }
        if (options & NMAP_MAC_ADDRESS) {
            XMLElement* mac = host->FirstChildElement("address");
            if (mac && mac->Attribute("addrtype", "mac")) {
                payload.mac_address = mac->Attribute("addr");
                has_data = true;
            }
        }
        if (options & NMAP_DOMAIN) {
            XMLElement* hostnames = host->FirstChildElement("hostnames");
            if (hostnames) {
                for (XMLElement* hostname = hostnames->FirstChildElement("hostname"); hostname; hostname = hostname->NextSiblingElement("hostname")) {
                    const char* domain = hostname->Attribute("name");
                    if (domain) {
                        payload.domain = domain;
                        has_data = true;
                    }
                }
            }
        }
        if (options & NMAP_OS) {
            XMLElement* os = host->FirstChildElement("os");
            if (os) {
                XMLElement* osmatch = os->FirstChildElement("osmatch");
                if (osmatch) {
                    const char* os_name = osmatch->Attribute("name");
                    if (os_name) {
                        payload.os = os_name;
                        has_data = true;
                    }
                }
            }
        }

        // If any data was collected, add it to results
        if (has_data) {
            results.push_back(payload);
        }

        host = host->NextSiblingElement("host");
    }
    return results;
}

std::vector<ScanPayload> Nmap::nmap_x(const std::string& target, uint8_t options) {
    try {
        std::string scan_result = run_nmap(target, options);
        return parse_nmap_xml(scan_result, options);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return {};
    }
}

std::vector<ScanPayload> Nmap::run(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cerr << "Usage: nmap_x <target> [options]" << std::endl;
        return {};
    }

    std::string target = args[1];
    uint8_t options = 0;

    std::unordered_map<std::string, uint8_t> option_map = {
        {"--open-ports", NMAP_OPEN_PORT},
        {"--public-ip", NMAP_PUBLIC_IP},
        {"--mac-address", NMAP_MAC_ADDRESS},
        {"--domain", NMAP_DOMAIN},
        {"--os", NMAP_OS}
    };

    for (size_t i = 2; i < args.size(); ++i) {
        auto it = option_map.find(args[i]);
        if (it != option_map.end()) {
            options |= it->second;
        }
    }

    std::vector<ScanPayload> results = nmap_x(target, options);

    // Print out the results (you can modify this to return or save as needed)
    for (const ScanPayload& payload : results) {
        std::cout << "IP: " << payload.ip << std::endl;
        std::cout << "MAC Address: " << payload.mac_address << std::endl;
        std::cout << "Domain: " << payload.domain << std::endl;
        std::cout << "OS: " << payload.os << std::endl;
        std::cout << "Open Ports: ";
        for (int port : payload.open_ports) {
            std::cout << port << " ";
        }
        std::cout << std::endl;
    }

    return results; // Return ScanPayloads instead of just strings
}
