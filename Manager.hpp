//
// Created by Eric on 2024-04-24.
//
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <ws2tcpip.h>

#ifndef UDPCHAT_MANAGER_HPP
#define UDPCHAT_MANAGER_HPP

#define NORMAL_MESSAGE 0
#define END_SERVER (-1)
#define END_SERVER_STR "END_SERVER"
#define LEAVE_SERVER 2
#define LEAVE_SERVER_STR "LEAVE_SERVER_"
#define GET_CLIENT_LIST_STR "GET_CLIENT_LIST"
#define GET_CLIENT_LIST 3

using std::cout;
using std::string;

struct Client {
    std::string NAME;
    std::string IP;
    sockaddr_in ADDRESS{};
    Client() {}
};

class Manager {
private:
    std::vector<Client> clients;
public:
    Manager();

    /* If this returns NORMAL_MESSAGE (0), it's a regular message. Otherwise,
     *
     * END_SERVER (-1): stops the server
     * LEAVE_SERVER (2): user left the server
     * buffer: users message
     */
    static int resolve(sockaddr_in& client, const char* buffer, Manager& manager);

    /* If true, initialize the referenced IP string the string's included IP
     *
     * ip: uninitialized at function entry
     * message: the users message
     */
    static bool leaving(string& ip, const string& message);

    /*
     * Called before Manager::resolve, this adds the client ADDRESS to the list if its new
     *
     * client: clients addr
     */
    void AddClientIfNew(sockaddr_in &client, const char* buffer);

    static string printClientList(Manager& manager);

    std::vector<Client> getClientList() {
        return clients;
    };
};

#endif //UDPCHAT_MANAGER_HPP
