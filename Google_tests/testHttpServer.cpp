#include "gtest/gtest.h"
#include "../src/centralServer/httpServer.hpp"

class HTTPServerTest : public ::testing::Test {
protected:
    virtual void SetUp()
    {
        testServer = new httpServer();
    }

    virtual void TearDown() {
        delete testServer;
    }

    httpServer* testServer;

};

TEST_F(HTTPServerTest, GETTestCorrect){

    std::string testString1 = "GET ";
    EXPECT_TRUE(testServer->checkForHTTPGET(testString1));
}

TEST_F(HTTPServerTest, GETTestFalse){

    std::string testString2 = "GOT ";
    EXPECT_FALSE(testServer->checkForHTTPGET(testString2));
}
