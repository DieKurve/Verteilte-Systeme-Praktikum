#include <iostream>
#include "fuelSensor.hpp"
#include "meanspeedSensor.hpp"
#include "speedSensor.hpp"
#include "trafficSensor.hpp"
#include <chrono>
#include <thread>


using namespace std::chrono_literals;

int main(int argc, char *argv[]) {

    std::string hostName = "mqttbroker";

    if (argc == 0) {
        std::cout << "Not enough arguments!" << std::endl;
        exit(-1);
    }

    fuelSensor *fuel;
    meanspeedSensor *meanSpeed;
    speedSensor *speed;
    trafficSensor *traffic;

    // we choose which sensor to create depending on the argument
    // which is defined in the docker-compose.yml
    int sensor = *argv[1] - '0';
    switch (sensor) {
        case 1:
            // create a new fuel sensor which then sends data every 2 seconds
            fuel = new fuelSensor(hostName, sensor);
            while (true) {
                std::this_thread::sleep_for(10s);
                fuel->sendData();
            }
        case 2:
            // create a new mean speed sensor which then sends data every 2 seconds
            meanSpeed = new meanspeedSensor(hostName, sensor);
            while (true) {
                std::this_thread::sleep_for(10s);
                meanSpeed->sendData();
            }
        case 3:
            // create a new speed sensor which then sends data every 2 seconds
            speed = new speedSensor(hostName, sensor);
            while (true) {
                std::this_thread::sleep_for(10s);
                speed->sendData();
            }
        case 4:
            // create a new traffic sensor which then sends data every 2 seconds
            traffic = new trafficSensor(hostName, sensor);
            while (true) {
                std::this_thread::sleep_for(10s);
                traffic->sendData();
            }
        default:
            std::cout << "Wrong argument!" << std::endl;
            break;
    }

    return 0;
}
