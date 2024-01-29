#ifndef VS_PRAKTIKUM_SRC_SENSORS_MEANSPEEDSENSOR_H_
#define VS_PRAKTIKUM_SRC_SENSORS_MEANSPEEDSENSOR_H_

#include "baseSensor.hpp"
#include <random>

/**
 *
 */

class meanspeedSensor : baseSensor {
public:
    meanspeedSensor();

    meanspeedSensor(const std::string &hostName, int id);

    ~meanspeedSensor() override;

    std::string generateMeanSpeed();

    void sendData() override;

    bool connectToServer();

    bool disconnectFromServer();

private:
    unsigned int meanSpeed = 0;


};


#endif //VS_PRAKTIKUM_SRC_SENSORS_MEANSPEEDSENSOR_H_
