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

    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    Client info;

    char buffer[BUFLEN];
    std::string message;

    bool setName = false;

    while (true) {
        if (!setName) {
            cout << "Name: ";
            std::getline(std::cin, info.NAME);
            if (info.NAME.empty()) {
                cout << "Name cannot be empty.\n";
                continue;
            }
            setName = true;
            sendto(s, info.NAME.c_str(), info.NAME.size(), 0, (sockaddr*)&server, sizeof(server));
        } else {
            cout << "Say: ";
            std::getline(std::cin, message);
            if (message.empty()) {
                continue;
            }
            sendto(s, message.c_str(), message.size(), 0, (sockaddr *) &server, sizeof(server));
        }
        sockaddr_in serverAddr;
        int serverAddrLen = sizeof(serverAddr);
        int bytesReceived = recvfrom(s, buffer, BUFLEN, 0, (sockaddr*)&serverAddr, &serverAddrLen);
        if (bytesReceived == SOCKET_ERROR) {
            std::cerr << "Error in recvfrom().";
            break;
        }
        buffer[bytesReceived] = '\0';
        std::cout << "> " << buffer << "\n";
    }

    closesocket(s);
    WSACleanup();

    return 0;
}
