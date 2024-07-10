#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

int serverSocket;
sockaddr_in serverAddress;

void checkConnections() {
    while (true) {
        fd_set rfds;
        struct timeval tv;

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        FD_ZERO(&rfds);
        FD_SET(serverSocket, &rfds);

        int recVal = select(serverSocket + 1, &rfds, NULL, NULL, &tv);
        switch (recVal) {
            case (0): {
                std::cout << "timeout" << std::endl;
                break;
            }
            case (-1): {
                std::cout << "error" << std::endl;
                break;
            }
            default: {
                std::cout << "defaulting" << std::endl;

                char buffer[1024] = {0};
                int sockLen = sizeof(serverAddress);
                if (recvfrom(serverSocket, buffer, sizeof(buffer), 0, (sockaddr*)&serverAddress, (socklen_t*)&sockLen) < 0) {
                    std::cout << "Failed to Recieve Data" << std::endl;
                    break;
                } else {
                    std::cout << "recieved data" << std::endl;
                }
                break;
            }
        }
    }
}

int main() {
    // creating socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);

    // specifying the address
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // binding socket.
    bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));

    // listening to the assigned socket
    listen(serverSocket, 5);

    // accepting connection request
    int clientSocket = accept(serverSocket, nullptr, nullptr);

    // checkConnections();

    // // recieving data

    while (true) {
        char buffer[1024] = {0};
        recv(clientSocket, buffer, sizeof(buffer), 0);
        std::cout << "Message from client: " << buffer
                  << std::endl;
    }
    char buffer[1024] = {0};
    recv(clientSocket, buffer, sizeof(buffer), 0);
    std::cout << "Message from client: " << buffer
              << '\n' << std::endl;

    // closing the socket.
    // close(serverSocket);

    return 0;
}
