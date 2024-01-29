#!/bin/bash

# 1. Check ob Apache Bench vorhanden

if test -f "/bin/ab"; then
	echo "Apache Bench exits."
else
	echo "Apache Bench does not exist! Please install Apache Bench!"
	exit
fi

if test -f "/bin/psql"; then
	echo "PostgreSQL interactive terminal exits."
else
	echo "PostgreSQL interactive terminal does not exits. Please install postgres-client!"
	exit
fi


if [ ! -d ".test/" ] ; then
	mkdir test
fi

if test -f "./test/testresults100.txt"; then
		rm ./test/testresults100.txt
fi


# 2. 100 Sensoren(je 25 pro Sensorentyp)
docker-compose up --scale fuelsensor=25 --scale meanspeedsensor=25 --scale speedsensor=25 --scale trafficsensor=25 &

echo "Generating Data"
sleep 80s

# 3. Testen mit Apache Bench | Jede Seite wird mit 100 Verbindungen, von denen immer 10 gleichzeitig stattfinden, getestet

ab -n 100 -c 10 http://localhost:8080/fuelsensor_1.csv >> ./test/testresults100.txt

ab -n 100 -c 10 http://localhost:8080/meanspeedsensor_2.csv >> ./test/testresults100.txt

ab -n 100 -c 10 http://localhost:8080/speedsensor_3.csv >> ./test/testresults100.txt

ab -n 100 -c 10 http://localhost:8080/trafficsensor_4.csv >> ./test/testresults100.txt

ab -n 100 -c 10 http://localhost:8080/allsensors.csv >> ./test/testresults100.txt


# 5. Testen mit 100 Sensoren| Überprüfen der Anzahl der gesendeten Daten innerhalb von einer Minute | Paketverlust von ~3% ist zu erwarten.

psql "host=localhost port=5432 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM fuelsensor' >> ./test/testresults100.txt
psql "host=localhost port=5432 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM meanspeedsensor' >> ./test/testresults100.txt
psql "host=localhost port=5432 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM speedsensor' >> ./test/testresults100.txt
psql "host=localhost port=5432 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM trafficsensor' >> ./test/testresults100.txt


# 4. Stop Container
docker-compose down

echo "Results in test folder in file testresults100.txt!"

#End of line
