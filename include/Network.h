#include <netinet/in.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <sys/socket.h>
#include <unistd.h>

#include <atomic>
#include <cstring>
#include <iostream>
#include <string>
#include <thread>
#include <arpa/inet.h>
#include "rapidjson/document.h"

class Network {
   public:
    int clientSocket;
    sockaddr_in serverAddress;

    void init();

    std::string createPacket(double speed, double forwards, double sideways);
    void sendInputPackets(std::atomic<double>& sideAxis, std::atomic<double>& forwardsAxis, std::atomic<double>& maxSpeed, std::atomic<bool>& enabled);
};