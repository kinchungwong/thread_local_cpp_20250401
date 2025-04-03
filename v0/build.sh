#!/bin/bash -e

g++ -std=c++11 -g -Og -DNDEBUG -o main.out main.cpp -I${PWD} -I${PWD}/include
objdump -d -C -M intel ./main.out > main.s
