#include "cli.hpp"
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

CLI::CLI(const std::string& host, int port) {
    // Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    // Convert IP address to binary form
    if (inet_pton(AF_INET, host.c_str(), &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // Send 'False' to indicate this is a CLI client
    std::string message = "0\n";
    send(sockfd, message.c_str(), message.length(), 0);
}

CLI::~CLI() {
    close(sockfd);
}

void CLI::run() {
    std::string command;
    while (true) {
        std::cout << "netmap> ";
        std::getline(std::cin, command);

        if (command == "exit") {
            break;
        }

        send_command(command);
    }
}

void CLI::send_command(const std::string& command) {
    // Send the command
    send(sockfd, command.c_str(), command.length(), 0);
    send(sockfd, "\n", 1, 0);

    // Receive the response
    char buffer[1024] = {0};
    int n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (n > 0) {
        buffer[n] = '\0';  // Null-terminate the string
        std::cout << "Server Response: " << buffer << std::endl;
    }
}