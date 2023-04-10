#!/bin/bash

make clean

cd build/

rm -rf *.cmake
rm -rf *.a
rm -rf *.a
rm -rf *.ini
rm -rf *.a
rm -rf *.json
rm *.lock
rm *.filters
rm *.vcxproj
rm *.bat
rm *.dir
rm Lab2
rm -rf Debug/
rm -rf x64/

rm -rf CMakeFiles/
rm -rf glad_cmake/
rm -rf gladsources/
rm -rf CMakeCache.txt
rm -rf conanbuildinfo.txt
rm -rf conaninfo.txt
rm -rf Makefile

cd ..

clear