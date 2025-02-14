#include "dig_x.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <stdexcept>
#include <array>

std::string DigX::dig_x(const std::string& domain, int options) {
    std::string raw_response = execute_dig(domain);

    std::string result;
    if (options & DIG_A_RECORD)      result += parse_a_record(raw_response);
    if (options & DIG_MX_RECORD)     result += parse_mx_record(raw_response);
    if (options & DIG_CNAME_RECORD)  result += parse_cname_record(raw_response);
    if (options & DIG_NS_RECORD)     result += parse_ns_record(raw_response);
    if (options & DIG_SOA_RECORD)    result += parse_soa_record(raw_response);

    return result.empty() ? "No matching records found\n" : result;
}

// Function to execute the dig command and return the output
std::string DigX::execute_dig(const std::string& domain) {
    std::string command = "dig " + domain + " ANY +short 2>&1";
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) throw std::runtime_error("Failed to execute dig command");

    char buffer[256];
    std::string result;
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }
    pclose(pipe);
    return result;
}

// Helper function to extract A record (IPv4)
std::string DigX::parse_a_record(const std::string& raw_response) {
    std::regex re("\\b([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})\\b");
    std::string result;
    std::sregex_iterator begin(raw_response.begin(), raw_response.end(), re);
    std::sregex_iterator end;

    for (auto it = begin; it != end; ++it) {
        result += "A Record: " + it->str() + "\n";
    }
    return result;
}

// Helper function to extract MX record
std::string DigX::parse_mx_record(const std::string& raw_response) {
    std::regex re("([0-9]+)\\s+IN\\s+MX\\s+([a-zA-Z0-9.-]+)");
    std::string result;
    std::sregex_iterator begin(raw_response.begin(), raw_response.end(), re);
    std::sregex_iterator end;

    for (auto it = begin; it != end; ++it) {
        result += "MX Record: " + (*it)[2].str() + " (Priority: " + (*it)[1].str() + ")\n";
    }
    return result;
}

// Helper function to extract CNAME record
std::string DigX::parse_cname_record(const std::string& raw_response) {
    std::regex re("CNAME\\s+([a-zA-Z0-9.-]+)");
    std::smatch match;
    if (std::regex_search(raw_response, match, re)) {
        return "CNAME Record: " + match[1].str() + "\n";
    }
    return "";
}

// Helper function to extract NS record
std::string DigX::parse_ns_record(const std::string& raw_response) {
    std::regex re("IN\\s+NS\\s+([a-zA-Z0-9.-]+)");
    std::string result;
    std::sregex_iterator begin(raw_response.begin(), raw_response.end(), re);
    std::sregex_iterator end;

    for (auto it = begin; it != end; ++it) {
        result += "NS Record: " + (*it)[1].str() + "\n";
    }
    return result;
}

// Helper function to extract SOA record
std::string DigX::parse_soa_record(const std::string& raw_response) {
    std::regex re("SOA\\s+([a-zA-Z0-9.-]+)\\s+([a-zA-Z0-9.-]+)");
    std::smatch match;
    if (std::regex_search(raw_response, match, re)) {
        return "SOA Record: " + match[1].str() + " " + match[2].str() + "\n";
    }
    return "";
}