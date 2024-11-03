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
mv centralserver docker/centralServer/

# 6. docker-compose build && docker-compose up

docker-compose build

echo "run the programm with docker-compose up"
