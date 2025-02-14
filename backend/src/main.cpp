#include "network_server.hpp"
#include <iostream>
#include <cstdlib>  // For std::stoi

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return 1;
    }

    try {
        // Parse the port number from the command line argument
        short port = std::stoi(argv[1]);

        // Create the server object and start it
        NetworkServer server(port);
        server.start();
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: Invalid port number. Please provide a valid integer." << std::endl;
        return 1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Error: Port number out of valid range." << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}