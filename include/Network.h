#include <cstring> 
#include <iostream> 
#include <netinet/in.h> 
#include <sys/socket.h> 
#include <unistd.h> 

class Network {
    public:
        int clientSocket;
        void sendPacket();
        void Start();
        void setRobotSpeed();

};