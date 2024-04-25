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

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8000

using std::cout;
using std::string;

std::mutex mtx;
std::condition_variable cv;
std::queue<std::string> queue;

bool running;

void receivePacket(SOCKET s, sockaddr_in& client, int& clientSize);
void sendPacket(SOCKET s, sockaddr_in& client, int& clientSize);

#endif //UDPCHAT_SERVER_HPP
