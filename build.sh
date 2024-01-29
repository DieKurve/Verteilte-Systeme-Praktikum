#!/bin/bash

# Hier kommt das Skript zum bauen des Projektes rein
# 1. Check CMake vorhanden
if test -f "/bin/cmake"; then
    echo "cmake exists."
    else
      echo "cmake does not exists! Please install cmake!"
      exit
fi
# 2. cmake CMakeLists.txt ausführen
cmake CMakeLists.txt
# 3. make ausführen
make

# 4. gradle ausführen
cd src/serviceProvider/
./gradlew distTar
cd build/distributions
tar -C ../../../../docker/serviceProvider -xvf serviceProvider.tar
cd ../../../..

# 5. ausführbare Dateien in die jeweilen Ordner in docker/ verschieben
mv sensor docker/sensors/
cp deb/libpaho-mqtt.cpp-1.2.0-Linux.deb docker/sensors
cp deb/libpaho-mqtt_1.3.8_amd64.deb docker/sensors

mv centralserver docker/centralServer/
cp deb/libpaho-mqtt.cpp-1.2.0-Linux.deb docker/centralserver
cp deb/libpaho-mqtt_1.3.8_amd64.deb docker/centralserver

# 6. docker-compose build && docker-compose up

docker-compose build

echo "run the programm with docker-compose up"
