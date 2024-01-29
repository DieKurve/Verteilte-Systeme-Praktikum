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

if test -f "./test/testresultsNonFunctional.txt"; then
		rm ./test/testresultsNonFunctional.txt
fi


# 2. Normaler System Start
docker-compose up &

sleep 25

# 3. Testen mit Apache Bench | Jede Seite wird mit 1000 Verbindungen, von denen immer 10 gleichzeitig stattfinden, getestet

ab -n 1000 -c 10 http://localhost:8080/fuelsensor_1.csv >> ./test/testresultsNonFunctional.txt

ab -n 1000 -c 10 http://localhost:8080/meanspeedsensor_2.csv >> ./test/testresultsNonFunctional.txt

ab -n 1000 -c 10 http://localhost:8080/speedsensor_3.csv >> ./test/testresultsNonFunctional.txt

ab -n 1000 -c 10 http://localhost:8080/trafficsensor_4.csv >> ./test/testresultsNonFunctional.txt

ab -n 1000 -c 10 http://localhost:8080/allsensors.csv >> ./test/testresultsNonFunctional.txt

docker-compose down


# 4. Testen mit 200 Fuelsensoren | Stabilitätstest des MQTT-Subscribers | Paketverlust von ~10% ist zu erwarten!

docker-compose up --scale fuelsensor=200 &

sleep 80s

psql "host=localhost port=5432 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM fuelsensor' >> ./test/testresultsNonFunctional.txt

# 5. Stop Container

docker-compose down

echo "Results in test folder in file testresultsNonFunctional.txt"

#End of line
