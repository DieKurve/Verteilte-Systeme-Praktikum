FROM ubuntu:latest
COPY centralserver /
RUN apt-get update
RUN apt-get install libthrift-0.19.0t64 libpaho-mqtt1.3 libpaho-mqtt_1.3.8_amd64.deb /
ENTRYPOINT ./centralserver