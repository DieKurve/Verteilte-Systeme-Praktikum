#include "fuelSensor.hpp"

using namespace std::chrono_literals;

fuelSensor::fuelSensor(const std::string &hostName, const int id) {
    name = "fuelSensor";
    this->id = id;
    mqttTopic = "sensor/" + name;
    std::string clientName = name + "_";
    clientName += std::to_string(createID());

    // create this sensor as new MQTT client
    mqttClient = createMQTTClient(clientName, hostName);
    connectToServer();

    std::cout << "Fuelsensor created!" << std::endl;
}

fuelSensor::~fuelSensor() = default;

/**
 * send the generated data to the MQTT server
 */
void fuelSensor::sendData() {
    mqtt::topic topic(*mqttClient, mqttTopic, QOS, true);

    if (mqttClient->is_connected()) {
        topic.publish(generateData(processFuel()));
    }
}

/**
 * reduces the value of current fuel by 1
 * @return string current fuel value
 */
std::string fuelSensor::processFuel() {
    if (currentFuel <= 0) {
        currentFuel = 100;
    }

    --currentFuel;

    std::string fuelData = std::to_string(currentFuel);

    return fuelData;
}

/**
 * connects the client to the MQTT server
 * @return true if successful, false if unsuccessful
 */
bool fuelSensor::connectToServer() {
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
bool fuelSensor::disconnectFromServer() {
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
