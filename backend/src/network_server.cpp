#include "network_server.hpp"
#include "commands.hpp"  // Universal import for all command header files
#include "network_scanner.hpp"
#include "network_state.hpp"
#include <iostream>
#include <sstream>
#include <vector>
#include <asio.hpp>

// Function to parse command input into a vector of arguments
std::vector<std::string> parse_command(const std::string& input) {
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    std::string token;
    while (iss >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

NetworkServer::NetworkServer(short port) 
    : io_context(), acceptor(io_context, tcp::endpoint(tcp::v4(), port)), port(port) {}

void NetworkServer::start() {
    std::cout << "Server started, listening on port" << port << "..." << std::endl;
    network_state.load_state();
    while (true) {
        tcp::socket socket(io_context);
        acceptor.accept(socket);
        std::thread(&NetworkServer::handle_client, this, std::move(socket)).detach();
    }
}

void NetworkServer::handle_client(tcp::socket socket) {
    try {
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

            if (command == "exit") {
                break;
            }

            std::vector<std::string> args = parse_command(command);
            if (args.empty()) {
                send_response(socket, "Invalid command");
                continue;
            }

            std::string response;
            std::string cmd = args[0];

            if (cmd == "ping") {
                response = Ping::run(args);
            } 
            else if (cmd == "traceroute") {
                response = Traceroute::run(args);
            } 
            else if (cmd == "nmap") {
                Nmap::run(args);
            }
            else if (cmd == "dig") {
                response = Dig::run(args);
            }
            else {
                response = "Unknown CLI command";
            }

            send_response(socket, response);
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
            std::string gui_data;
            std::getline(input, gui_data);

            if (gui_data == "exit") {
                break;
            }

            std::cout << "Received GUI data: " << gui_data << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "GUI client error: " << e.what() << std::endl;
    }
}

void NetworkServer::send_response(tcp::socket& socket, const std::string& response) {
    asio::write(socket, asio::buffer(response + "\n"));
}