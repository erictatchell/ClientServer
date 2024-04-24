//
// Created by Eric on 2024-04-24.
//

#include "client.hpp"

int main() {
    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);

    sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server.sin_addr);

    SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

    char buffer[BUFLEN];
    std::string message;

    while (true) {
        std::cout << "Enter message: ";
        std::getline(std::cin, message);
        sendto(s, message.c_str(), message.size(), 0, (sockaddr*)&server, sizeof(server));

        sockaddr_in serverAddr;
        int serverAddrLen = sizeof(serverAddr);
        int bytesReceived = recvfrom(s, buffer, BUFLEN, 0, (sockaddr*)&serverAddr, &serverAddrLen);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Error in recvfrom().";
            break;
        }

        buffer[bytesReceived] = '\0';
        std::cout << "Server says: " << buffer << std::endl;
    }

    closesocket(s);
    WSACleanup();

    return 0;
}
