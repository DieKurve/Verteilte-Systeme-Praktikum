#include "gtest/gtest.h"


#define protected public
#define private public
#include "../src/sensors/baseSensor.hpp"
#undef protected
#undef private

#include "../src/sensors/fuelSensor.hpp"

class sensorTest : public ::testing::Test {
protected:
    virtual void SetUp()
    {
        testSensor = new fuelSensor("",5);
    }

    virtual void TearDown() {
        delete testSensor;
    }

    fuelSensor* testSensor = nullptr;
};

TEST_F(sensorTest, WrongConnection){
    std::string test1 = "testBroker";
    testSensor = new fuelSensor(test1,1);
    EXPECT_FALSE(testSensor->connectToServer());

}


TEST_F(sensorTest, CorrectConnection){
    std::string test2 = "localhost";
    testSensor = new fuelSensor(test2,1);
    EXPECT_TRUE(testSensor->connectToServer());
    testSensor->disconnectFromServer();
}
