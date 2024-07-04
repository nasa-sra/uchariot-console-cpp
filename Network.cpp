#include "include/Network.h"

void Network::Start()
{
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    // while (true)
    // {
    //     int error_code;
    //     socklen_t error_code_size = sizeof(error_code);
    //     std::cout << getsockopt(clientSocket, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size) << std::endl;
    // }

    // const char *message = "Hello, server!";
    // send(clientSocket, message, strlen(message), 0);

    // close(clientSocket);
}

void Network::sendPacket()
{
    std::cout << "sending message" << std::endl;
    const char *message = "Hello, server!";
    send(clientSocket, message, strlen(message), 0);
    std::cout << "sent message" << std::endl;
}

// void Network::
