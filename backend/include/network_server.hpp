#ifndef NETWORK_SERVER_HPP
#define NETWORK_SERVER_HPP

#include <asio.hpp>
#include <string>
#include <thread>

using asio::ip::tcp;

class NetworkServer {
public:
    // Constructor takes a port for the server to listen on
    NetworkServer(short port);

    // Start the server to listen for incoming connections
    void start();

private:
    // Handle each client connection
    void handle_client(tcp::socket socket);

    // Send response to the client
    void send_response(tcp::socket& socket, const std::string& response);

    asio::io_context io_context;
    tcp::acceptor acceptor;
};

#endif // NETWORK_SERVER_HPP