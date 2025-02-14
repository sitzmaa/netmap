#ifndef CLI_HPP
#define CLI_HPP

#include <string>

class CLI {
public:
    CLI(const std::string& host, int port);
    ~CLI();

    void run();

private:
    int sockfd;

    void send_command(const std::string& command);
};

#endif // CLI_HPP