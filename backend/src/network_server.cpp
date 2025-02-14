#include "network_server.hpp"
#include <iostream>
#include <asio.hpp>

NetworkServer::NetworkServer(short port) 
    : io_context(), acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {}

void NetworkServer::start() {
    std::cout << "Server started, listening on port 12345..." << std::endl;
    while (true) {
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::thread(&NetworkServer::handle_client, this, std::move(socket)).detach();
    }
}

void NetworkServer::handle_client(tcp::socket socket) {
    try {
        std::cout << "Client connected" << std::endl;

        // Determine if the client is a GUI or CLI
        bool is_gui = false;
        asio::streambuf buffer;
        asio::read_until(socket, buffer, "\n");
        std::istream input(&buffer);
        input >> is_gui;

        std::cout << (is_gui ? "GUI client connected" : "CLI client connected") << std::endl;

        // Now process the commands based on the client type
        while (true) {
            asio::streambuf buffer;
            asio::read_until(socket, buffer, "\n");

            std::istream input(&buffer);
            std::string command;
            std::getline(input, command);

            if (command == "ping") {
                send_response(socket, "Pinging the network...");
            } else if (command == "traceroute") {
                send_response(socket, "Running traceroute...");
            } else if (command == "exit") {
                break;
            } else {
                send_response(socket, "Unknown command");
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error handling client: " << e.what() << std::endl;
    }
}

void NetworkServer::send_response(tcp::socket& socket, const std::string& response) {
    asio::write(socket, asio::buffer(response + "\n"));
}