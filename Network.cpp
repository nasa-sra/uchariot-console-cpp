#include "include/Network.h"

int clientSocket;

void Network::Start()
{
    // creating socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // sending connection request
    connect(clientSocket, (struct sockaddr *)&serverAddress,
            sizeof(serverAddress));

    // sending data
    const char *message = "Hello, server!";
    send(clientSocket, message, strlen(message), 0);

    // closing socket
    close(clientSocket);

}

void Network::sendPacket()
{
    std::cout << "sending message" << std::endl;
    const char *message = "Hello, server!";
    send(clientSocket, message, strlen(message), 0);
    std::cout << "sent message" << std::endl;

}    

