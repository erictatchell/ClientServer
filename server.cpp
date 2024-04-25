#include "server.hpp"
#include "Manager.hpp"

void receivePacket(SOCKET s, sockaddr_in& client, int& clientSize) {
    while (running) {
        char buffer[BUFLEN];
        int bytes = recvfrom(s, buffer, BUFLEN, 0, (struct sockaddr*)&client, &clientSize);
        if (bytes > 0) {
            std::lock_guard<std::mutex> lock(mtx);

            int message = Manager::resolve(client, buffer);

            if (message == NORMAL_MESSAGE) {
                queue.emplace(buffer);
                cout << "Received: " << buffer << '\n';
                cv.notify_one();
            } else if (message == END_SERVER) {
                running = false;
                break;
            }
        }
    }
}

void sendPacket(SOCKET s, sockaddr_in& client, int& clientSize) {
    while (running) {
        {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, []{
                return !queue.empty();
            });
            string message = queue.front();
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

    server.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server.sin_addr); // change the ip in the hpp's
    server.sin_port = htons(SERVER_PORT); // change the port in the hpp's

    SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (bind(s, (sockaddr*)&server, sizeof(server)) == 0) {
        running = true;
        std::cout << "Server started on " << SERVER_IP << ":" << SERVER_PORT << '\n';
    }

    std::thread receiveThread(receivePacket, s, std::ref(client), std::ref(clientSize));
    std::thread sendThread(sendPacket, s, std::ref(client), std::ref(clientSize));

    receiveThread.join();
    sendThread.join();

    closesocket(s);
    WSACleanup();

    return 0;
}
