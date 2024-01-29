#include "trafficSensor.hpp"

trafficSensor::trafficSensor(const std::string &hostName, const int id) {
    name = "trafficsensor";
    this->id = id;
    mqttTopic = "sensor/" + name;
    std::string clientName = name + "_";
    clientName += std::to_string(createID());

    // create this sensor as new MQTT client
    mqttClient = createMQTTClient(clientName, hostName);
    connectToServer();

    std::cout << "Trafficsensor created!" << std::endl;
}

trafficSensor::~trafficSensor() = default;

/**
 * send the generated data to the MQTT server
 */
void trafficSensor::sendData() {
    mqtt::topic topic(*mqttClient, mqttTopic, QOS, true);

    if (mqttClient->is_connected()) {
        topic.publish(generateData(generateTraffic()));
    }
}

/**
 * generates a random int between 1 and 4 to represent traffic situation
 * @return current traffic situation
 */
std::string trafficSensor::generateTraffic() {
    std::random_device randDev;
    std::default_random_engine randomTraffic(randDev());
    std::uniform_int_distribution<int> uniform_dist(1, 4);
    currentTraffic = uniform_dist(randomTraffic);
    std::string currentTrafficString = std::to_string(currentTraffic);

    return currentTrafficString;
}

/**
 * connects the client to the MQTT server
 * @return true if successful, false if unsuccessful
 */
bool trafficSensor::connectToServer() {
    try {
        std::cout << "Connecting to server Mosquitto'" << "'..." << std::flush;
        mqttClient->connect(mqttConnectOptions)->wait();
        std::cout << "OK\n" << std::endl;

        return true;
    } catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;

        return false;
    }
}

/**
 * disconnects the client from the MQTT server
 * @return true if successful, false if unsuccessful
 */
bool trafficSensor::disconnectFromServer() {
    try {
        std::cout << "\nDisconnecting..." << std::flush;
        mqttClient->disconnect()->wait();
        std::cout << "OK" << std::endl;

        return true;
    } catch (const mqtt::exception &exc) {
        std::cerr << exc.what() << std::endl;

        return false;
    }
}
