#!/bin/bash
# build a c++ file with default parameters
# use like ./build mrmw

if [ "$1" = "" ]; then
  set "tests" # if missing param build test
fi

set "${1%.*}"

mkdir -p ../bin
rm -f ../bin/$1

c++ "${1}.cc" -Wfatal-errors -lpthread -O3 -std=c++17 -I ../lib -I ../libs -o ../bin/$1
if [ $? -eq 0 ]; then
    time ../bin/$1
else
    echo BUILD FAILED  
fi
