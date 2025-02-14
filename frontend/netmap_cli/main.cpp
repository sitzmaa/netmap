#include "cli.hpp"
#include <iostream>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <cstdlib>
#include <arpa/inet.h>

void display_help() {
    std::cout << "Usage: netmap_cli [options]\n";
    std::cout << "Options:\n";
    std::cout << "  -h <host>         Specify the target IP address (default: localhost)\n";
    std::cout << "  -H <host domain>  Specify the target domain to resolve to an IP\n";
    std::cout << "  -p <port>         Specify the port (default: 12345)\n";
    std::cout << "  --help            Display this help message\n";
}

std::string resolve_hostname(const std::string& domain) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET; // IPv4
    hints.ai_socktype = SOCK_STREAM;

    int err = getaddrinfo(domain.c_str(), nullptr, &hints, &res);
    if (err != 0) {
        std::cerr << "Error resolving domain: " << gai_strerror(err) << std::endl;
        exit(EXIT_FAILURE);
    }

    // Convert the first resolved address to a string
    char ip_str[INET_ADDRSTRLEN];
    struct sockaddr_in* sockaddr_in = (struct sockaddr_in*)res->ai_addr;
    inet_ntop(AF_INET, &sockaddr_in->sin_addr, ip_str, sizeof(ip_str));

    freeaddrinfo(res); // Free the resolved info
    return std::string(ip_str);
}

int main(int argc, char* argv[]) {
    int port = 12345;
    std::string host = "localhost"; // Default IP address
    bool resolve_dns = false;
    std::string domain;

    // Handle command-line arguments
    int opt;
    while ((opt = getopt(argc, argv, "h:H:p:")) != -1) {
        switch (opt) {
        case 'h':
            host = optarg;
            break;
        case 'H':
            resolve_dns = true;
            domain = optarg;
            break;
        case 'p':
            port = std::stoi(optarg);
            break;
        case '?':
            display_help();
            return EXIT_SUCCESS;
        default:
            display_help();
            return EXIT_FAILURE;
        }
    }

    // Check for --help flag
    if (argc == 2 && std::string(argv[1]) == "--help") {
        display_help();
        return EXIT_SUCCESS;
    }

    // If DNS resolution is requested, resolve the domain
    if (resolve_dns) {
        host = resolve_hostname(domain);
        std::cout << "Resolved DNS: " << domain << " to IP: " << host << std::endl;
    }

    // Create and run the CLI
    CLI cli(host, port);
    cli.run();

    return 0;
}