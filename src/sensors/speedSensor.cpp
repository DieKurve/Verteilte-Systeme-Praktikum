#include "speedSensor.hpp"

speedSensor::speedSensor(const std::string &hostName, const int id) {
    name = "speedSensor";
    this->id = id;
    mqttTopic = "sensor/" + name;
    std::string clientName = name + "_";
    clientName += std::to_string(createID());

    // create this sensor as new MQTT client
    mqttClient = createMQTTClient(clientName, hostName);
    connectToServer();

    std::cout << "Speedsensor created!" << std::endl;
}

speedSensor::~speedSensor() = default;

/**
 * send the generated data to the MQTT server
 */
void speedSensor::sendData() {
    mqtt::topic topic(*mqttClient, mqttTopic, QOS, true);

    if (mqttClient->is_connected()) {
        topic.publish(generateData(generateSpeed()));
    }
}

/**
* generates a random int value between 50 and 140
* @return string generated current speed value
*/
std::string speedSensor::generateSpeed() {
    std::random_device randDev;
    std::default_random_engine randomSpeed(randDev());
    std::uniform_int_distribution<int> uniform_dist(50, 140);
    currentSpeed = uniform_dist(randomSpeed);
    std::string returnString = std::to_string(currentSpeed);

    return returnString;
}

/**
 * connects the client to the MQTT server
 * @return true if successful, false if unsuccessful
 */
bool speedSensor::connectToServer() {
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
bool speedSensor::disconnectFromServer() {
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
