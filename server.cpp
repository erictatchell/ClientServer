#include "server.hpp"

/* If true, initialize the referenced IP string the string's included IP
 *
 * ip: uninitialized at function entry, to be i
 * message: the users message
 */
bool leaving(string& ip, const string& message) {
    string strLEAVE = LEAVE_SERVER; // so we can perform string operations
    if (message.compare(0, strLEAVE.size(), strLEAVE) == 0) {
        // the IP address is everything after "LEAVE_SERVER_"
        ip = message.substr(strLEAVE.size());
        return true;
    }
    return false;
}

void handleMessage() {

}

int checkMessage(const char* buffer) {
    if (string(buffer) == END_SERVER_STRING) {
        cout << "END_SERVER received, quitting\n";
        running = false;
        return END_SERVER;
    }

    string IP;
    if (leaving(IP, buffer) {
        // remove matching IP client from vector
        cout << "Client " << IP << "left the server\n";
        return 2;
    }
    return 0;
}

void receivePacket(SOCKET s, sockaddr_in& client, int& clientSize) {
    while (running) {
        char buffer[BUFLEN];
        int bytes = recvfrom(s, buffer, BUFLEN, 0, (struct sockaddr*)&client, &clientSize);
        if (bytes > 0) {
            std::lock_guard<std::mutex> lock(mtx);
            int message = checkMessage(buffer);
            if (message == NORMAL) {
                queue.emplace(buffer);
                cout << "Received: " << buffer << '\n';
                cv.notify_one();
            } else if (message == END_SERVER) break;
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
