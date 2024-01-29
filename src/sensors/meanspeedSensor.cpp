#include "meanspeedSensor.hpp"

meanspeedSensor::meanspeedSensor(const std::string &hostName, const int id) {
    name = "meanspeedSensor";
    this->id = id;
    mqttTopic = "sensor/" + name;
    std::string clientName = name + "_";
    clientName += std::to_string(createID());

    // create this sensor as new MQTT client
    mqttClient = createMQTTClient(clientName, hostName);
    connectToServer();

    std::cout << "Meanspeedsensor created!" << std::endl;
}

meanspeedSensor::~meanspeedSensor() = default;

/**
 * send the generated data to the MQTT server
 */
void meanspeedSensor::sendData() {
    mqtt::topic topic(*mqttClient, mqttTopic, QOS, true);

    if (mqttClient->is_connected()) {
        topic.publish(generateData(generateMeanSpeed()));
    }
}

/**
 * generates a random int value between 50 and 140
 * @return string generated meanspeed value
 */
std::string meanspeedSensor::generateMeanSpeed() {
    std::random_device randDev;
    std::default_random_engine randomSpeed(randDev());
    std::uniform_int_distribution<int> uniform_dist(50, 140);
    meanSpeed = uniform_dist(randomSpeed);
    std::string stringMeanSpeed = std::to_string(meanSpeed);

    return stringMeanSpeed;
}

/**
 * connects the client to the MQTT server
 * @return true if successful, false if unsuccessful
 */
bool meanspeedSensor::connectToServer() {
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
bool meanspeedSensor::disconnectFromServer() {
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
