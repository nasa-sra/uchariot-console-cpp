#include "Network.h"

void Network::init() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    int connection = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
}

int recursionNum = 0;
void Network::connectToChariot() {
    recursionNum++;
    // std::cout << "connecting to chariot server... (" << unsigned(recursionNum) << std::endl;
    if (recursionNum > 2) {
        init();
    };
    int connection = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    if (connection == 0) {
        std::cout << "connection succesful." << std::endl;
        const char *message = "Hello, server!";
        send(clientSocket, message, strlen(message), 0);

    } else if (connection == -1 && recursionNum < 500) {
        // std::cout << "connection failed." << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        connectToChariot();
    } else {
        std::cout << "connection ultimately failed." << std::endl;
    }

    // while (getConnectionStatus() == 0) {
    //     connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    //     // std::cout << "CONNECTION FAILURE" << std::endl;
    // }
}

int Network::getConnectionStatus() {
    int error_code;
    socklen_t error_code_size = sizeof(error_code);
    return getsockopt(clientSocket, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
}

void Network::sendPacket(std::atomic<double> &sideAxis, std::atomic<double> &forwardsAxis, std::atomic<bool> &enabled) {
    while (true) {
        double forwardsAxisValue = 0;
        double sideAxisValue = 0;
        if (enabled) {
            forwardsAxisValue = forwardsAxis.load();
            sideAxisValue = sideAxis.load();
        }
        std::string mess = "Forwards Axis: " + std::to_string(forwardsAxisValue) + " Side Axis: " + std::to_string(sideAxisValue);
        const char *message = mess.c_str();
        send(clientSocket, message, strlen(message), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
