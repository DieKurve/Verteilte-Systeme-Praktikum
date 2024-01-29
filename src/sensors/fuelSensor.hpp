#ifndef VS_PRAKTIKUM_SRC_FUELSENSOR_H
#define VS_PRAKTIKUM_SRC_FUELSENSOR_H

#include <iostream>
#include <netdb.h>
#include <string>
#include "baseSensor.hpp"
#include <gtest/gtest.h>

/**
 *
 */

class fuelSensor : baseSensor {
    friend class sensorTest;

public:
    fuelSensor(const std::string &hostName, int id);

    ~fuelSensor() override;

    bool connectToServer();

    void sendData() override;

    bool disconnectFromServer();

    std::string processFuel();

private:
    FRIEND_TEST(sensorTest, WrongConnection);

    FRIEND_TEST(sensorTest, CorrectConnection);

    unsigned int currentFuel = 100;

};

#endif //VS_PRAKTIKUM_SRC_FUELSENSOR_H
