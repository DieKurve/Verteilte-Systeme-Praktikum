# Verteilte Systeme Praktikum

Die Anforderungen sind unter ```Anforderungen.pdf``` im Wurzelverzeichnis zu finden.
Die Systemarchitektur ist unter ```Architektur.pdf``` im Wurzelverzeichnis zu finden.
Die Lizenzdatei ist unter ```LICENSE``` im Wurzelverzeichnis zu finden.

Das ```docker-compose.yml``` File ist im Wurzelverzeichnis abgelegt und die Docker Files sind im docker Verzeichnis in den jeweiligen Unterverzeichnissen zu finden.

Der Quellcode des gesamten Projekts befindet sich im src Verzeichnis.


# Installation

Folgende Software muss zum Kompilieren installiert sein:
- Docker Version 19.03
- Docker-Compose Version 3.7
- CMake Version 3.13
- GCC Version 9
- Java 11
- paho-mqtt-c++ Bibliothek v.1.3.0 (https://github.com/eclipse-paho/paho.mqtt.cpp)

Zur Generierung der Dokumentation muss Doxygen installiert sein und mit dem Skript doc.sh kann die Dokumentation des Projekts generiert werden


Um das System zu installieren und auszuführen sind folgende Schritte durchzuführen:
1. Skript ```build.sh``` im Wurzelverzeichnis ausführen
2. System mit ```docker-compose up``` starten
3. Verbindung zu "localhost:8080/[parameter]"  
    Folgende Parameter sind verfügbar:
    - allsensors.csv - zeigt Werte aller Sensoren an
    - fuelsensor_1.csv - zeigt alle Werte des Tankfüllstandssensors an
    - meanspeedsensor_2.csv - zeigt alle Werte für die Durchschnittsgeschwindigkeit an
    - speedsensor_3.csv - zeigt alle Werte des Geschwindigkeitssensors an
    - trafficsensor_4.csv - zeigt alle Werte der Verkehrssituation an


# Testen
Voraussetzung für das Testen sind zusätzlich ApacheBench und postgresql client.

Die Tests können mittels Bash Skripts ausgeführt werden.  

Zunächst muss ```build.sh``` mindestens einmal ausgeführt worden sein.  
Anschließend können die funktionalen Tests mit ```testCode.sh``` ausgeführt werden. 
Die Testergebnisse befinden sich nach der Ausführung im test Verzeichnis.  
Die Performance Tests liegen in zwei Varianten vor: ```test10.sh``` und ```test100.sh```.   
Zu Beginn von beiden Tests wird jeweils ein Zentralserver mit einem HTTP Server gestartet.
Im Test ```test10.sh``` werden noch zusätzlich je Sensorentyp 2 beziehungsweise 3 Sensoren gestartet.  
Auf jede URI wird mit 10 Verbindungen gleichzeitig einmal zugegriffen. 
Im zweiten Performancetest ```test100.sh``` werden jeweils 25 Sensoren jedes Typs gestartet.
Auf jede URI wird mit 10 Verbindungen gleichzeitig 10 mal zugegriffen.  

Des Weiteren wird mit ```test10.sh``` der Paketverlust innerhalb einer Minute von den Sensoren
bis zum Serviceprovider getestet, wobei in Verlust von etwa 3% zu erwarten ist.
Mit ```test100.sh``` ist mit ungefähr 8% Paketverlust zu rechnen.

Zusätzlich gibt es noch einen nicht-funktionalen Test ```testNonFunctional.sh```.  
Zu Beginn des Tests wird das System gestart. Im Anschluss greift der Test auf jede URI mit 10 gleichzeitigen Verbindungen 100 Mal zu.
Um den MQTT Subscriber zu testen werden 200 Sensoren vom Typ fuelsensor gestartet. Der Paketverlust
beläuft sich auf ungefähr 10%.

Um die Synchronisation zwischen den Serviceprovidern nach einem simulierten Ausfall zu testen, wurde ein JUnit Test entwickelt, der überprüft, ob die Daten in den Datenbanken der Serviceprovider nach einem Ausfall identisch sind. Dieser Test kann mit dem Skript ```testServiceProviderSync.sh``` ausgeführt werden.
