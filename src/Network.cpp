#include "Network.h"

void Network::init() {
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8001);
    // serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_addr.s_addr = inet_addr("10.93.24.9");

    int connection = connect(clientSocket, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
}

std::string Network::createPacket(double speed, double forwards, double sideways) {
    rapidjson::Document d;
    d.SetObject();

    rapidjson::Document::AllocatorType &allocator = d.GetAllocator();

    size_t sz = allocator.Size();

    d.AddMember("speed", speed, allocator);
    d.AddMember("fwd", forwards, allocator);
    d.AddMember("turn", sideways, allocator);

    rapidjson::StringBuffer strbuf;
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(strbuf);
    d.Accept(writer);

    std::string buff = strbuf.GetString();
    std::string buff2 = "[teleop_drive] " + buff + ";";
    return buff2;
}

void Network::sendInputPackets(std::atomic<double> &sideAxis, std::atomic<double> &forwardsAxis, std::atomic<double> &maxSpeed, std::atomic<bool> &enabled) {
    while (true) {
        double forwardsAxisValue = 0;
        double sideAxisValue = 0;
        if (enabled) {
            forwardsAxisValue = forwardsAxis.load();
            sideAxisValue = sideAxis.load();
        }

        if (abs(forwardsAxisValue) < .01) forwardsAxisValue = 0;
        if (abs(sideAxisValue) < .01) sideAxisValue = 0;

        std::string buff = createPacket(maxSpeed.load(), forwardsAxisValue, sideAxisValue);
        const char *message = buff.c_str();
        send(clientSocket, message, strlen(message), 0);
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}
