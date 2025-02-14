#ifndef DIG_X_HPP
#define DIG_X_HPP

#include <string>

// Define bitwise flags for Dig options
#define DIG_A_RECORD       (1 << 0)  // 00001
#define DIG_MX_RECORD      (1 << 1)  // 00010
#define DIG_CNAME_RECORD   (1 << 2)  // 00100
#define DIG_NS_RECORD      (1 << 3)  // 01000
#define DIG_SOA_RECORD     (1 << 4)  // 10000

// Function that interacts with dig and parses results based on bitwise options
std::string dig_x(const std::string& domain, int options);

// Helper functions to parse dig response
std::string parse_a_record(const std::string& raw_response);
std::string parse_mx_record(const std::string& raw_response);
std::string parse_cname_record(const std::string& raw_response);
std::string parse_ns_record(const std::string& raw_response);
std::string parse_soa_record(const std::string& raw_response);

#endif // DIG_X_HPP