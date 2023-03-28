#!/bin/bash

# make -e PROGRAM=bin/main.exe

# ./bin/main.exe
cd build/

conan install . --build missing


cmake --build .

cd ../

./build/Lab2