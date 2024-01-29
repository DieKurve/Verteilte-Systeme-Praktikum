cmake_minimum_required(VERSION 3.13)
project(VS_Praktikum)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
add_subdirectory(Google_tests)

find_package(PahoMqttCpp REQUIRED)

add_executable(sensor
        src/sensors/main.cpp
        src/sensors/baseSensor.hpp
        src/sensors/baseSensor.cpp
        src/sensors/fuelSensor.hpp
        src/sensors/fuelSensor.cpp
        src/sensors/meanspeedSensor.cpp
        src/sensors/meanspeedSensor.hpp
        src/sensors/speedSensor.hpp
        src/sensors/speedSensor.cpp
        src/sensors/trafficSensor.hpp
        src/sensors/trafficSensor.cpp
        /usr/local/include/mqtt/async_client.h
        )

target_link_libraries(sensor PahoMqttCpp::paho-mqttpp3)

add_executable(centralserver
        src/centralServer/httpServer.cpp
        src/centralServer/httpServer.hpp
        src/centralServer/centralServer.cpp
        src/centralServer/centralServer.hpp
        src/gen-cpp/getCentralData.cpp
        src/gen-cpp/service_constants.cpp
        src/gen-cpp/service_types.cpp
        src/centralServer/main.cpp)

target_link_libraries(centralserver PahoMqttCpp::paho-mqttpp3 )
find_library(THRIFT thrift)

target_link_libraries(centralserver ${THRIFT})

add_library(httpServer STATIC
        src/centralServer/httpServer.cpp
        src/centralServer/httpServer.hpp
        )

add_library(fuelsensorLib STATIC
        src/sensors/baseSensor.hpp
        src/sensors/baseSensor.cpp
        src/sensors/fuelSensor.hpp
        src/sensors/fuelSensor.cpp)
