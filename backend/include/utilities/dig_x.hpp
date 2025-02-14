#ifndef DIG_X_HPP
#define DIG_X_HPP

#include <string>

#define DIG_A_RECORD     0x01
#define DIG_MX_RECORD    0x02
#define DIG_CNAME_RECORD 0x04
#define DIG_NS_RECORD    0x08
#define DIG_SOA_RECORD   0x10

class DigX {
public:
    static std::string dig_x(const std::string& domain, int options);

private:
    static std::string execute_dig(const std::string& domain);
    static std::string parse_a_record(const std::string& raw_response);
    static std::string parse_mx_record(const std::string& raw_response);
    static std::string parse_cname_record(const std::string& raw_response);
    static std::string parse_ns_record(const std::string& raw_response);
    static std::string parse_soa_record(const std::string& raw_response);
};

#endif // DIG_X_HPP