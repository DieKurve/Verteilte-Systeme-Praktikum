# 'Google_test' is the subproject name
project(Google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# adding the Google_Tests_run target
add_executable(Google_Tests_run testHttpServer.cpp testSensoren.cpp)

target_link_libraries(Google_Tests_run httpServer)

find_package(PahoMqttCpp REQUIRED)
target_link_libraries(Google_Tests_run fuelsensorLib PahoMqttCpp::paho-mqttpp3)

target_link_libraries(Google_Tests_run gtest gtest_main)