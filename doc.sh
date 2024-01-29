#!/bin/bash

# Hier kommt das Skript zum bauen des Projektes rein
# 1. Check Doxygen vorhanden
if test -f "/bin/doxygen"; then
    echo "doxygen exists."
    else
      echo "doxygen does not exists! Please install doxygen!"
      exit
fi
# 2. doxygen Doxyfile aufführen
doxygen Doxyfile

firefox ./doc/html/index.html