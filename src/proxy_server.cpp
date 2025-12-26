#include "proxy_server.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <thread>
ProxyServer::ProxyServer(const std::string& host, int port)
    : host_(host), port_(port), server_fd_(-1) {}

void ProxyServer::setup_socket() {
    server_fd_ = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd_ < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    setsockopt(server_fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port_);
    inet_pton(AF_INET, host_.c_str(), &addr.sin_addr);

    if (bind(server_fd_, (sockaddr*)&addr, sizeof(addr)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd_, 50) < 0) {
    perror("listen failed");
    exit(EXIT_FAILURE);
    }
}
    
void ProxyServer::start() {
    setup_socket();
    std::cout << "Proxy listening on " << host_ << ":" << port_ << std::endl;
    
    while (true) {
        sockaddr_in client_addr{};
        socklen_t len = sizeof(client_addr);
    
        int client_fd = accept(server_fd_, (sockaddr*)&client_addr, &len);
        if (client_fd < 0) {
            perror("accept failed");
            continue;
        }
        std::thread([client_fd]() {
            char buffer[1024];
             recv(client_fd, buffer, sizeof(buffer), 0);
            close(client_fd);
        }).detach();
    }
}
