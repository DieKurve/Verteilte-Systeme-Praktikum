#include "baseSensor.hpp"

baseSensor::baseSensor() = default;

baseSensor::~baseSensor() {
    // disconnect the MQTT client and destroy the object
    if (mqttClient != nullptr && mqttClient->is_connected()) {
        mqttClient->disconnect()->wait();
    }

    delete mqttClient;
}

/**
 * creates an MQTT client as publisher for communication with the central server
 * @return a reference to the MQTT client, null if unsuccessful
 */
mqtt::async_client *baseSensor::createMQTTClient(const std::string &topicClient, const std::string &hostName) {
    try {
        const auto PERIOD = std::chrono::seconds(10);

        // 60 * 10sec => 10min off-line buffering
        const int MAX_BUFFERED_MSGS = 60;

        // create the MQTT client which is used to communicate with the MQTT server
        auto *newMQTTClient = new mqtt::async_client(hostName, topicClient, MAX_BUFFERED_MSGS);

        mqttConnectOptions = mqtt::connect_options_builder()
                .keep_alive_interval(MAX_BUFFERED_MSGS * PERIOD)
                .clean_session(true)
                .automatic_reconnect(true)
                .finalize();

        return newMQTTClient;
    } catch (std::exception &e) {
        std::cerr << &e << std::endl;
    }

    return nullptr;
}

/**
 * returns the id of the sensor
 * @return int sensor id
 */
unsigned int baseSensor::getId() const {
    return id;
}

/**
 * generates the current data for transmission
 * @return string data package with sensorID, data, timestamp
 */
std::string baseSensor::generateData(const std::string &data) {
    std::stringstream dataStream{};

    // concatenate the sensor id, the data to transmit and the current timestamp
    dataStream << getId() << ",";
    dataStream << data << ",";
    auto currentTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    dataStream << currentTime.time_since_epoch().count() << "\n";

    return dataStream.str();
}

/**
 * generates a random ID for the sensor
 * @return int sensorID
 */
unsigned int baseSensor::createID() {
    std::random_device randDev;
    std::default_random_engine randomTraffic(randDev());
    std::uniform_int_distribution<int> uniform_dist(1, INT32_MAX - 10);
    return uniform_dist(randomTraffic);
}
