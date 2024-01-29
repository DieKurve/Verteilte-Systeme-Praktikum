#ifndef VS_PRAKTIKUM_SRC_TRAFFICSENSOR_H
#define VS_PRAKTIKUM_SRC_TRAFFICSENSOR_H

#include "baseSensor.hpp"

/**
 *
 */

class trafficSensor : baseSensor {
public:
    trafficSensor(const std::string &hostName, int id);

    virtual ~trafficSensor();

    void sendData() override;

    bool connectToServer();

    bool disconnectFromServer();

    std::string generateTraffic();

private:
    /**
     * Frei = 1, mäßiger Verkehr = 2, schwerer Verkehr = 3, Stau = 4
     */

    unsigned int currentTraffic = 1;

};

#endif //VS_PRAKTIKUM_SRC_TRAFFICSENSOR_H
