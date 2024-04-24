#include "server.hpp"

void receivePacket(SOCKET s, sockaddr_in& client, int& clientSize) {
    while (true) {
        char buffer[BUFLEN];
        int bytes = recvfrom(s, buffer, BUFLEN, 0, (struct sockaddr*)&client, &clientSize);
        if (bytes > 0) {
            std::lock_guard<std::mutex> lock(mtx);
            queue.emplace(buffer);
            cout << "Received: " << buffer << '\n';
            cv.notify_one();
        }
    }
}

void sendPacket(SOCKET s, sockaddr_in& client, int& clientSize) {
    while (true) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []{
                return !queue.empty();
            });
            std::string message = queue.front();
            queue.pop();
            if (sendto(s, message.c_str(), message.size(), 0, (struct sockaddr*)&client, clientSize) > -1) {
                cout << "Sent: " << message << '\n';
            }
        }
    }
}

int main() {
    sockaddr_in server, client;
    int clientSize = sizeof(client);

    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);

    const char* ip = "127.0.0.1"; // Server IP address
    unsigned int port = 8000;

    server.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &server.sin_addr);
    server.sin_port = htons(port);

    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (bind(s, (sockaddr*)&server, sizeof(server)) == 0) {
        std::cout << "Server started on " << ip << ":" << port << '\n';
    }

    std::thread receiveThread(receivePacket, s, std::ref(client), std::ref(clientSize));
    std::thread sendThread(sendPacket, s, std::ref(client), std::ref(clientSize));

    receiveThread.join();
    sendThread.join();

    closesocket(s);
    WSACleanup();

    return 0;
}
