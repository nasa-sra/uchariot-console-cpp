#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <string>
#include <atomic>
#include <thread>
#include <string>

class Network {
   public:
    int clientSocket;
    sockaddr_in serverAddress;

    void init();
    int getConnectionStatus();

    void connectToChariot();
    void sendPacket(std::atomic<double>& sideAxis, std::atomic<double>& forwardsAxis);
    void setRobotSpeed();
};