#include "Network.h"

void Network::init() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8001);
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

std::string Network::createPacket(int speed, double forwards, double sideways) {
    rapidjson::Document d;
    d.SetObject();

    rapidjson::Document::AllocatorType &allocator = d.GetAllocator();

    size_t sz = allocator.Size();

    d.AddMember("speed", 2500, allocator);
    d.AddMember("fwd", forwards, allocator);
    d.AddMember("turn", sideways, allocator);

    rapidjson::StringBuffer strbuf;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
    d.Accept(writer);

    std::string buff = strbuf.GetString();
    std::string buff2 = "[teleop_drive] " + buff + ";";
    return buff2;
}

void Network::sendPacket(std::atomic<double> &sideAxis, std::atomic<double> &forwardsAxis, std::atomic<bool> &enabled) {
    while (true) {
        double forwardsAxisValue = 0;
        double sideAxisValue = 0;
        if (enabled) {
            forwardsAxisValue = forwardsAxis.load();
            sideAxisValue = sideAxis.load();
        }

        if (abs(forwardsAxisValue) < .01) forwardsAxisValue = 0;
        if (abs(sideAxisValue) < .01) sideAxisValue = 0;

        std::string buff = createPacket(2500, forwardsAxisValue, sideAxisValue);
        const char *message = buff.c_str();
        send(clientSocket, message, strlen(message), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
