
#!/bin/bash

cd build/

conan install . build_type=Debug --build missing

cmake .. -DCMAKE_BUILD_TYPE=Debug

cmake --build .

cd ../

./build/Lab2
