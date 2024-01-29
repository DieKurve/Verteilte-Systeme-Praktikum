#ifndef VS_PRAKTIKUM_SRC_BASESENSOR_H
#define VS_PRAKTIKUM_SRC_BASESENSOR_H

#include <unistd.h>
#include <string>
#include <iostream>
#include <random>
#include <netdb.h>
#include <chrono>
#include <string>
#include <sstream>
#include "mqtt/async_client.h"

/**
 *
 */

class baseSensor {
public:
    baseSensor();

    virtual ~baseSensor();

    unsigned int getId() const;

protected:

    std::string generateData(const std::string &data);

    unsigned int createID();

    mqtt::async_client* createMQTTClient(const std::string& topicClient, const std::string& hostName);


    /**
     *
     * @param toSend
     */
    virtual void sendData() = 0;


    /**
     * Name of the sensor
     */
    std::string name{};

    /**
     * Id of the sensor
     */
    unsigned int id = 65535;

    mqtt::async_client* mqttClient = nullptr;

    mqtt::connect_options mqttConnectOptions;

    std::string mqttTopic = {};

    const int QOS = 1;

    std::string serverURI = {};
};

#endif //VS_PRAKTIKUM_SRC_BASESENSOR_H
