FROM ubuntu:latest
COPY sensor /
RUN apt-get update
RUN apt-get install libthrift-0.19.0t64 libpaho-mqtt1.3 libpaho-mqttpp3-1 -y
ENTRYPOINT ./sensor