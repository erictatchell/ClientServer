//
// Created by Eric on 2024-04-24.
//
#include "Manager.hpp"

bool Manager::leaving(string& ip, const string& message) {
    string strLEAVE = LEAVE_SERVER_STR; // so we can perform string operations
    if (message.compare(0, strLEAVE.size(), strLEAVE) == 0) {
        // the IP address is everything after "LEAVE_SERVER_"
        ip = message.substr(strLEAVE.size());
        return true;
    }
    return false;
}

void Manager::AddClientIfNew(sockaddr_in& client) {
    bool found = false;
    for (const auto& info : clients) {
        if (memcmp(&info.address, &client, sizeof(client)) == 0) {
            found = true;
            break;
        }
    }
    if (!found) {
        Client c;
        c.address = client;
        clients.push_back(c);
    }
}

int Manager::resolve(sockaddr_in& client, const char* buffer, Manager& manager) {
    manager.AddClientIfNew(client);

    if (string(buffer) == END_SERVER_STR) {
        cout << "END_SERVER received, quitting\n";
        return END_SERVER;
    }

    string IP;
    if (Manager::leaving(IP, buffer)) {
        // remove matching IP client from vector

        cout << "Client " << IP << "left the server\n";
        return LEAVE_SERVER;
    }
    return NORMAL_MESSAGE;
}

Manager::Manager() {
    clients = std::vector<Client>();
}

