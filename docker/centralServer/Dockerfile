FROM ubuntu:latest
COPY centralserver /
RUN apt update && apt install libthrift-0.13.0 -y
COPY libpaho-mqtt.cpp-1.2.0-Linux.deb /
COPY libpaho-mqtt_1.3.8_amd64.deb /
RUN apt update && apt install ./libpaho-mqtt.cpp-1.2.0-Linux.deb ./libpaho-mqtt_1.3.8_amd64.deb -y
ENTRYPOINT ./centralserver
