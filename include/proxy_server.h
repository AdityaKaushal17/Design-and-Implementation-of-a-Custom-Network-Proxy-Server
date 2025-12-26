#pragma once
#include <string>

class ProxyServer {
public:
    ProxyServer(const std::string& host, int port);
    void start();

private:
    std::string host_;
    int port_;
    int server_fd_;

    void setup_socket();
};
