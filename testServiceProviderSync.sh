#!/bin/bash

sudo docker-compose up &
echo "Waiting for docker containers ..."
sleep 40

echo "Waiting random time between 10 and 30 seconds to simulate failure ..."
sleep $(shuf -i 10-30 -n 1)

sudo docker-compose stop serviceproviderprimary

echo "Waiting random time between 70 and 90 seconds to start the service provider ..."
sleep $(shuf -i 70-90 -n 1)

sudo docker-compose start serviceproviderprimary

echo "Checking database contents ..."
sleep 10

rm ./test/fuelsensordb1.txt
rm ./test/meanspeedsensordb1.txt
rm ./test/speedsensordb1.txt
rm ./test/trafficsensordb1.txt

rm ./test/fuelsensordb2.txt
rm ./test/meanspeedsensordb2.txt
rm ./test/speedsensordb2.txt
rm ./test/trafficsensordb2.txt

psql "host=localhost port=5432 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM fuelsensor' >> ./test/fuelsensordb1.txt
psql "host=localhost port=5432 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM meanspeedsensor' >> ./test/meanspeedsensordb1.txt
psql "host=localhost port=5432 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM speedsensor' >> ./test/speedsensordb1.txt
psql "host=localhost port=5432 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM trafficsensor' >> ./test/trafficsensordb1.txt

psql "host=localhost port=5433 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM fuelsensor' >> ./test/fuelsensordb2.txt
psql "host=localhost port=5433 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM meanspeedsensor' >> ./test/meanspeedsensordb2.txt
psql "host=localhost port=5433 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM speedsensor' >> ./test/speedsensordb2.txt
psql "host=localhost port=5433 dbname=praktikumdb user=postgres password=vspraktikum" -c 'SELECT COUNT(*) FROM trafficsensor' >> ./test/trafficsensordb2.txt

fsDiff=$(diff "./test/fuelsensordb1.txt" "./test/fuelsensordb2.txt")
msDiff=$(diff "./test/meanspeedsensordb1.txt" "./test/meanspeedsensordb2.txt")
ssDiff=$(diff "./test/speedsensordb1.txt" "./test/speedsensordb2.txt")
tsDiff=$(diff "./test/trafficsensordb1.txt" "./test/trafficsensordb2.txt")


if $fsDiff
then
  echo "Test successful!"
fi

if $msDiff
then
  echo "Test successful!"
fi

if $ssDiff
then
  echo "Test successful!"
fi

if $tsDiff
then
  echo "Test successful!"
fi

sudo docker-compose down

