#include "dig_x.hpp"
#include <iostream>
#include <sstream>
#include <regex>
#include <stdexcept>
#include "dns_lookup.hpp"  // Assuming dns_lookup.hpp defines the base DNS lookup function

std::string dig_x(const std::string& domain, int options) {
    // Call the base DNS lookup function (assumed to execute dig)
    std::string raw_response = dns_lookup(domain);

    std::string result;
    if (options & DIG_A_RECORD)      result += parse_a_record(raw_response);
    if (options & DIG_MX_RECORD)     result += parse_mx_record(raw_response);
    if (options & DIG_CNAME_RECORD)  result += parse_cname_record(raw_response);
    if (options & DIG_NS_RECORD)     result += parse_ns_record(raw_response);
    if (options & DIG_SOA_RECORD)    result += parse_soa_record(raw_response);

    return result.empty() ? "No matching records found\n" : result;
}

// Helper function to extract A record (IPv4)
std::string parse_a_record(const std::string& raw_response) {
    std::regex re("\\b([0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3})\\b");
    std::smatch match;
    if (std::regex_search(raw_response, match, re)) {
        return "A Record: " + match[1].str() + "\n";
    }
    return "";
}

// Helper function to extract MX record
std::string parse_mx_record(const std::string& raw_response) {
    std::regex re("([0-9]+)\\s+IN\\s+MX\\s+([a-zA-Z0-9.-]+)");
    std::smatch match;
    if (std::regex_search(raw_response, match, re)) {
        return "MX Record: " + match[2].str() + " (Priority: " + match[1].str() + ")\n";
    }
    return "";
}

// Helper function to extract CNAME record
std::string parse_cname_record(const std::string& raw_response) {
    std::regex re("CNAME\\s+([a-zA-Z0-9.-]+)");
    std::smatch match;
    if (std::regex_search(raw_response, match, re)) {
        return "CNAME Record: " + match[1].str() + "\n";
    }
    return "";
}

// Helper function to extract NS record
std::string parse_ns_record(const std::string& raw_response) {
    std::regex re("IN\\s+NS\\s+([a-zA-Z0-9.-]+)");
    std::smatch match;
    if (std::regex_search(raw_response, match, re)) {
        return "NS Record: " + match[1].str() + "\n";
    }
    return "";
}

// Helper function to extract SOA record
std::string parse_soa_record(const std::string& raw_response) {
    std::regex re("SOA\\s+([a-zA-Z0-9.-]+)\\s+([a-zA-Z0-9.-]+)");
    std::smatch match;
    if (std::regex_search(raw_response, match, re)) {
        return "SOA Record: " + match[1].str() + " " + match[2].str() + "\n";
    }
    return "";
}