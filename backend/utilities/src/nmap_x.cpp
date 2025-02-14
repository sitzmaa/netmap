#include "nmap_x.hpp"
#include <iostream>
#include <sstream>
#include <vector>
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

std::vector<std::string> parse_nmap_xml(const std::string& xml_data, uint8_t options) {
    XMLDocument doc;
    std::vector<std::string> results;
    if (doc.Parse(xml_data.c_str()) != XML_SUCCESS) {
        throw std::runtime_error("Failed to parse XML data from nmap");
    }

    XMLElement* root = doc.RootElement();
    if (!root) {
        throw std::runtime_error("Failed to find root element in nmap XML");
    }

    XMLElement* host = root->FirstChildElement("host");
    while (host) {
        if (options & NMAP_OPEN_PORT) {
            XMLElement* ports = host->FirstChildElement("ports");
            if (ports) {
                for (XMLElement* port = ports->FirstChildElement("port"); port; port = port->NextSiblingElement("port")) {
                    const char* port_str = port->Attribute("portid");
                    if (port_str) results.push_back("Open Port: " + std::string(port_str));
                }
            }
        }
        if (options & NMAP_PUBLIC_IP) {
            XMLElement* address = host->FirstChildElement("address");
            if (address) {
                const char* ip = address->Attribute("addr");
                if (ip) results.push_back("Public IP: " + std::string(ip));
            }
        }
        if (options & NMAP_MAC_ADDRESS) {
            XMLElement* mac = host->FirstChildElement("address");
            if (mac && mac->Attribute("addrtype", "mac")) {
                results.push_back("MAC Address: " + std::string(mac->Attribute("addr")));
            }
        }
        if (options & NMAP_DOMAIN) {
            XMLElement* hostnames = host->FirstChildElement("hostnames");
            if (hostnames) {
                for (XMLElement* hostname = hostnames->FirstChildElement("hostname"); hostname; hostname = hostname->NextSiblingElement("hostname")) {
                    const char* domain = hostname->Attribute("name");
                    if (domain) results.push_back("Domain: " + std::string(domain));
                }
            }
        }
        if (options & NMAP_OS) {
            XMLElement* os = host->FirstChildElement("os");
            if (os) {
                XMLElement* osmatch = os->FirstChildElement("osmatch");
                if (osmatch) {
                    const char* os_name = osmatch->Attribute("name");
                    if (os_name) results.push_back("OS: " + std::string(os_name));
                }
            }
        }
        host = host->NextSiblingElement("host");
    }
    return results;
}

std::vector<std::string> nmap_x(const std::string& target, uint8_t options) {
    try {
        std::string scan_result = run_nmap(target, options);
        return parse_nmap_xml(scan_result, options);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return {};
    }
}

std::string Nmap::run(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        return "Usage: nmap_x <target> [options]";
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

    std::vector<std::string> results = nmap_x(target, options);
    for (const std::string& res : results) {
        std::cout << res << std::endl;
    }
    return 0;
}
