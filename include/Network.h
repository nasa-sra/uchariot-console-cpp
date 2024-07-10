#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include <atomic>
#include <thread>
#include <string>
#include "rapidjson/document.h"
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>


class Network {
   public:
    int clientSocket;
    sockaddr_in serverAddress;

    void init();
    int getConnectionStatus();

    std::string createPacket(int speed, double forwards, double sideways);
    void connectToChariot();
    void sendPacket(std::atomic<double>& sideAxis, std::atomic<double>& forwardsAxis, std::atomic<bool>& enabled);
    void setRobotSpeed();
};