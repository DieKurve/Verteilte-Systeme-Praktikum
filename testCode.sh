#!/bin/bash

# 1. Check ob CMake vorhanden ist
if test -f "/bin/cmake"; then
    	echo "cmake exists."
    else
      	echo "cmake does not exist! Please install cmake!"
      exit
fi

if [ ! -d ".test/" ] ; then
	mkdir test
fi

if test -f "./test/testresultsCode.txt"; then
		rm ./test/testresultsCode.txt
fi

# 2. cmake CMakeLists.txt ausführen
cmake CMakeLists.txt
# 3. make ausführen
make

# 4. Test ausführen
Google_tests/Google_Tests_run >> ./test/testresultsCode.txt


echo "Results in test folder in file testresultsCode.txt!"

# 5. Serviceprovider Test ausführen
cd src/serviceProvider/
./gradlew test

#End of line
