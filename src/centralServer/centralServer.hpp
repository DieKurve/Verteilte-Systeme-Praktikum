#ifndef VS_PRAKTIKUM_CENTRALSERVER_HPP
#define VS_PRAKTIKUM_CENTRALSERVER_HPP

#include "httpServer.hpp"

#include <arpa/inet.h>
#include <cstring>
#include <vector>
#include <filesystem>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <unistd.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>
#include "../gen-cpp/getCentralData.h"
#include "mqtt/async_client.h"

class centralServer {
public:

    centralServer();

    virtual ~centralServer();

    httpServer *getCurrentHttpServer() const;

    void receivePackets();

private:

    bool isTime(int timePoint);

    mqtt::async_client *createMQTTClient();

    void saveToCSVFile(std::string &saveData);

    void syncToProvider();

    mqtt::async_client *mqttClient = nullptr;

    mqtt::connect_options mqttConnectOptions;

    std::vector<serviceProvider::sensorData> bufferRPC;

    std::string mqttTopic = "#";

    const int QOS = 1;

    httpServer *currentHttpServer;

    std::chrono::time_point<std::chrono::steady_clock> time = std::chrono::steady_clock::now();
};


#endif //VS_PRAKTIKUM_CENTRALSERVER_HPP
