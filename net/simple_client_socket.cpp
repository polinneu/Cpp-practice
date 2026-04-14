#include <array>
#include <cerrno>
#include <cstring>
#include <format>
#include <iostream>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>
#include <poll.h>
#include <shared_mutex>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cout << "Unable to create socket" << std::endl;
        return 0;
    }
    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080); //порт компа/сервера
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr); //IPшник
    if (connect(sock, (sockaddr *) & server_addr, sizeof(server_addr)) < 0) {
        std::cout << "Unable to connect to server" << std::endl;
        close(sock);
        return 0;
    }

    std::string msg = "Hello, server!";
    ssize_t sent = send(sock, msg.c_str(), msg.size(), 0);
    if (sent < 0) {
        std::cout << "Unable to send message" << std::endl;
        close(sock);
        return 0;
    }

    char buffer[1024];
    int bytes = recv(sock, buffer, 1024, 0);
    if (bytes <= 0) {
        std::cout << "Unable to receive message" << std::endl;
        close(sock);
        return 0;
    }
    for (size_t i = 0; i < bytes; i++) {
        std::cout << buffer[i];
    }

    close(sock);
}
