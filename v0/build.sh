#!/bin/bash -e

mkdir -p build
g++ -std=c++11 -g -Og -DNDEBUG -o build/main.out main.cpp -I${PWD} -I${PWD}/include
objdump -d -C -M intel ./build/main.out > ./build/main.s
