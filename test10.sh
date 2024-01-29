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

if test -f "./test/testresults10.txt"; then
		rm ./test/testresults10.txt
fi


# 2. 10 Sensoren(je 2 bzw. 3 pro Sensorentyp)
docker-compose up --scale fuelsensor=2 --scale meanspeedsensor=3 --scale speedsensor=2 --scale trafficsensor=3 &

# 3. Warten bis Daten generiert wurden
echo "Generating Data"
sleep 80s

# 4. Testen mit Apache Bench | Jede Seite wird mit 10 Verbindungen getestet, die gleichzeitig auf die Seiten zugreifen

ab -n 10 -c 10 http://localhost:8080/fuelsensor_1.csv >> ./test/testresults10.txt

ab -n 10 -c 10 http://localhost:8080/meanspeedsensor_2.csv >> ./test/testresults10.txt

ab -n 10 -c 10 http://localhost:8080/speedsensor_3.csv >> ./test/testresults10.txt

ab -n 10 -c 10 http://localhost:8080/trafficsensor_4.csv >> ./test/testresults10.txt

ab -n 10 -c 10 http://localhost:8080/allsensors.csv >> ./test/testresults10.txt


# 5. Testen mit 10 Sensoren| Überprüfen der Anzahl der gesendeten Daten innerhalb von einer Minute | Paketverlust ist nicht zu erwarten

psql "host=localhost port=5432 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM fuelsensor' >> ./test/testresults10.txt
psql "host=localhost port=5432 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM meanspeedsensor' >> ./test/testresults10.txt
psql "host=localhost port=5432 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM speedsensor' >> ./test/testresults10.txt
psql "host=localhost port=5432 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM trafficsensor' >> ./test/testresults10.txt

# 6. Stop Container
docker-compose down

echo "Results in test folder in file testresults10.txt!"

#End of line
