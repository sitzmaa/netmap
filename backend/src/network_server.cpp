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

        // Read initial message to determine client type
        asio::streambuf buffer;
        asio::read_until(socket, buffer, "\n");
        std::istream input(&buffer);

        std::string client_type;
        std::getline(input, client_type);

        if (client_type == "1") {
            std::cout << "GUI client connected" << std::endl;
            handle_gui(std::move(socket));
        } 
        else if (client_type == "0") {
            std::cout << "CLI client connected" << std::endl;
            handle_cli(std::move(socket));
        } 
        else {
            std::cerr << "Unknown client type: " << client_type << ". Closing connection." << std::endl;
            socket.close();
        }
    } catch (const std::exception& e) {
        std::cerr << "Error handling client: " << e.what() << std::endl;
    }
}

void NetworkServer::handle_cli(tcp::socket socket) {
    try {
        while (true) {
            asio::streambuf buffer;
            asio::read_until(socket, buffer, "\n");

            std::istream input(&buffer);
            std::string command;
            std::getline(input, command);

            if (command == "ping") {
                send_response(socket, "Pinging the network...");
            } 
            else if (command == "traceroute") {
                send_response(socket, "Running traceroute...");
            } 
            else if (command == "exit") {
                break;
            } 
            else {
                send_response(socket, "Unknown CLI command");
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "CLI client error: " << e.what() << std::endl;
    }
}

void NetworkServer::handle_gui(tcp::socket socket) {
    try {
        while (true) {
            asio::streambuf buffer;
            asio::read_until(socket, buffer, "\n");

            std::istream input(&buffer);
            std::string request;
            std::getline(input, request);

            // GUI messages might be JSON or structured differently
            if (request == "get_network_state") {
                send_response(socket, "{ \"status\": \"ok\", \"message\": \"Network state fetched\" }");
            } 
            else if (request == "highlight_node") {
                send_response(socket, "{ \"status\": \"ok\", \"message\": \"Node highlighted\" }");
            } 
            else if (request == "exit") {
                break;
            } 
            else {
                send_response(socket, "{ \"status\": \"error\", \"message\": \"Unknown GUI request\" }");
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "GUI client error: " << e.what() << std::endl;
    }
}

void NetworkServer::send_response(tcp::socket& socket, const std::string& response) {
    asio::write(socket, asio::buffer(response + "\n"));
}