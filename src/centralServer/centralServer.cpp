#include "centralServer.hpp"

using namespace apache::thrift;
using namespace apache::thrift::protocol;
using namespace apache::thrift::transport;

centralServer::centralServer() {
    // create the MQTT client which is used to communicate with the MQTT server
    mqttClient = createMQTTClient();

    // create the HTTP server
    currentHttpServer = new httpServer();
}

centralServer::~centralServer() {
    if (mqttClient->is_connected()) {

        std::cout << "\nShutting down and disconnecting from the MQTT server..." << std::endl;
        mqttClient->unsubscribe("#")->wait();
        mqttClient->stop_consuming();
        mqttClient->disconnect()->wait();
        std::cout << "OK" << std::endl;
    } else {
        std::cout << "\nClient was disconnected" << std::endl;
    }

    delete mqttClient;
    delete currentHttpServer;
}

/**
 * creates an MQTT Client which listens to the broker
 * * @return a reference to the MQTT client
 */
mqtt::async_client *centralServer::createMQTTClient() {
    const auto PERIOD = std::chrono::seconds(5);


    // 120 * 5sec => 10min off-line buffering
    const int MAX_BUFFERED_MSGS = 120;

    const std::string PERSIST_DIR{"data-persist"};

    // create the MQTT client which is used to communicate with the MQTT server

    auto *newMQTTClient = new mqtt::async_client("mqttbroker", mqttTopic, MAX_BUFFERED_MSGS);

    mqttConnectOptions = mqtt::connect_options_builder()
            .clean_session(true)
            .automatic_reconnect(true)
            .finalize();

    return newMQTTClient;
}

/**
 * receives udp packets from the sensors and saves the data to their csv files
 * for the purpose of printing the history on the http server
 */
void centralServer::receivePackets() {
    try {
        mqttClient->start_consuming();

        std::cout << "Connecting to the MQTT server...  ";
        auto mqttToken = mqttClient->connect(mqttConnectOptions);

        // Getting the connect response will block waiting for the
        // connection to complete.
        auto response = mqttToken->get_connect_response();

        // subscribe to the MQTT server to be able to receive messages
        if (!response.is_session_present())
            mqttClient->subscribe("#", QOS)->wait();

        std::cout << "OK" << std::endl;

        while (true) {
            auto buffer = mqttClient->consume_message();

            if (!buffer) {
                continue;
            }

            std::cout << buffer->get_topic() << ": " << buffer->to_string() << std::flush;
            std::string saveData(buffer->to_string());
            saveToCSVFile(saveData);

            if (isTime(1)) {
                syncToProvider();
                bufferRPC.clear();
            }
        }
    }
    catch (const mqtt::exception &exc) {
        std::cerr << "\n  " << exc << std::endl;
    }
}

/**
 * Syncs sensor data with the serviceprovider
 */
void centralServer::syncToProvider() {
    bool primaryFailed = false, secondaryFailed = false;
    std::shared_ptr<TTransport> socket;

    while (!secondaryFailed) {
        if (!primaryFailed) {
            socket = std::shared_ptr<TTransport>(new TSocket("serviceproviderprimary", 9876));
        } else {
            socket = std::shared_ptr<TTransport>(new TSocket("serviceprovidersecondary", 9876));
        }

        std::shared_ptr<TTransport> transport(new TBufferedTransport(socket));
        std::shared_ptr<TProtocol> protocol(new TBinaryProtocol(transport));
        serviceProvider::getCentralDataClient client(protocol);

        try {
            transport->open();
            client.pingServer();
            std::cout << "pingServer()" << std::endl;
            client.sendData(bufferRPC);
            bufferRPC.clear();
            socket->close();
            break;
        } catch (TException &tx) {
            if (primaryFailed) {
                secondaryFailed = true;
                std::cerr << "Could not write to any serviceprovider!" << std::endl;
            }
            primaryFailed = true;
        }
    }
}

/**
 *
 * @param saveData given data which is being saved in a csv file for the sensor
 */
void centralServer::saveToCSVFile(std::string &saveData) {
    std::stringstream fileStream(saveData);
    std::vector<std::string> saveFile;
    std::string tmp;

    while (std::getline(fileStream, tmp, ',')) {
        saveFile.emplace_back(tmp);
    }

    if (!fileStream && tmp.empty()) {
        saveFile.emplace_back("");
    }

    if (saveFile.size() != 3) {
        std::cout << "Saving failed!";
        return;
    }

    std::string head = "id, value, timestamp\n";
    int newId = std::stoi(saveFile.at(0));
    std::string saveName{};

    switch (newId) {
        case 1:
            saveName = "fuelsensor_";
            saveName.append(saveFile.at(0));
            break;
        case 2:
            saveName = "meanspeedsensor_";
            saveName.append(saveFile.at(0));
            break;
        case 3:
            saveName = "speedsensor_";
            saveName.append(saveFile.at(0));
            break;

        case 4:
            saveName = "trafficsensor_";
            saveName.append(saveFile.at(0));
            break;

        default:
            std::cerr << "Saving failed!" << std::endl;
            return;

    }
    saveName.append(".csv");
    std::ofstream saveCSVFile;
    if (!std::filesystem::exists(saveName)) {
        saveCSVFile.open(saveName);
        saveCSVFile << head;
        saveCSVFile.close();
    }
    saveCSVFile.open(saveName, std::fstream::app);
    if (!saveCSVFile.is_open()) {
        std::cerr << "Saving failed!" << std::endl;
        return;
    }
    saveCSVFile << saveData;
    serviceProvider::sensorData data;

    try {
        data.id = std::stoi(saveFile.at(0));
        data.value = std::stoi(saveFile.at(1));
        data.timestamp = saveFile.at(2);
    }
    catch (const std::invalid_argument &conversionError) {
        std::cerr << "Invalid argument: " << conversionError.what() << std::endl;
    }

    bufferRPC.emplace_back(data);
}

/**
 *
 * @return reference to HTTP server object
 */
httpServer *centralServer::getCurrentHttpServer() const {
    return currentHttpServer;
}

bool centralServer::isTime(int timePoint) {
    auto currentTime = std::chrono::steady_clock::now();
    if (currentTime >= time + std::chrono::minutes(timePoint)) {
        time = currentTime;
        return true;
    }
    return false;
}
