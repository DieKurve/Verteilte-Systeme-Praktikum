#ifndef VS_PRAKTIKUM_SRC_SPEEDSENSOR_H_
#define VS_PRAKTIKUM_SRC_SPEEDSENSOR_H_

#include <random>
#include "baseSensor.hpp"

/**
 *
 */

class speedSensor : baseSensor {
public:
    speedSensor(const std::string &hostName, int id);

    ~speedSensor() override;

    void sendData() override;

    bool connectToServer();

    bool disconnectFromServer();

    std::string generateSpeed();

private:
    unsigned int currentSpeed = 100;
};

#endif //VS_PRAKTIKUM_SRC_SPEEDSENSOR_H_
