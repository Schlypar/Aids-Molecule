#!/bin/bash

cd build/

# conan install . --build missing

cmake -S .. -B . -DCMAKE_BUILD_TYPE=Release

cmake --build .

cd ../

./build/Lab2-3-4