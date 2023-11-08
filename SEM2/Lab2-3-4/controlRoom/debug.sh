#!/bin/bash

cd build/

cmake -S .. -B . -DCMAKE_BUILD_TYPE=Debug

cmake --build . -- -silent

cd ../

valgrind -q ./build/Lab2-3-4 

