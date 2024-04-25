//
// Created by Eric on 2024-04-24.
//

#ifndef UDPCHAT_SERVER_HPP
#define UDPCHAT_SERVER_HPP
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <thread>
#include <mutex>
#include <vector>
#include <queue>
#include <condition_variable>

#define BUFLEN 512
#define NORMAL 0
#define END_SERVER (-1)
#define END_SERVER_STRING "END_SERVER"
#define LEAVE_SERVER "LEAVE_SERVER_"
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000

using std::cout;
using std::string;

typedef struct {
    std::string name;
} Client;

std::mutex mtx;
std::condition_variable cv;
std::queue<std::string> queue;

bool running;

// for LEAVE_SERVER
bool leaving(const std::string& str, const std::string& prefix);
int checkMessage(const char* buffer);
void receivePacket(SOCKET s, sockaddr_in& client, int& clientSize);
void sendPacket(SOCKET s, sockaddr_in& client, int& clientSize);

#endif //UDPCHAT_SERVER_HPP
