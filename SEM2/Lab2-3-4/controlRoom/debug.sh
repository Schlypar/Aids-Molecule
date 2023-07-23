#!/bin/bash

cd build/

cmake -S .. -B . -DCMAKE_BUILD_TYPE=Debug

cmake --build .

cd ../

valgrind ./build/Lab2-3-4
