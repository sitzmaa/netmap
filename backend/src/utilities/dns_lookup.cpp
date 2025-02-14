#include "dns_lookup.hpp"
#include <iostream>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

std::string dns_lookup(const std::string& hostname) {
    struct hostent *he = gethostbyname(hostname.c_str());
    if (he == nullptr) return "DNS lookup failed";
    return std::string(inet_ntoa(*(struct in_addr*)he->h_addr));
}