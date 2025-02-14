#ifndef DIG_X_HPP
#define DIG_X_HPP

#include "commands.hpp"
#include <vector>
#include <string>

// Define bitwise flags for Dig options
#define DIG_A_RECORD     (1 << 0)  // Extract A record
#define DIG_MX_RECORD    (1 << 1)  // Extract MX record
#define DIG_CNAME_RECORD (1 << 2)  // Extract CNAME record
#define DIG_NS_RECORD    (1 << 3)  // Extract NS record
#define DIG_SOA_RECORD   (1 << 4)  // Extract SOA record

class Dig{
public:
    static std::string run(const std::vector<std::string>& args);
};

#endif // DIG_X_HPP