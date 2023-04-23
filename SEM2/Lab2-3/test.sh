#!/bin/bash

cd build/

cmake -S .. -B . -DCMAKE_BUILD_TYPE=Debug

cmake --build .

cd ../

./build/tests
