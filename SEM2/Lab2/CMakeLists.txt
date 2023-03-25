cmake_minimum_required(VERSION 3.22.1)

project(Lab2)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake) 

conan_basic_setup()

link_directories(${CONAN_LIB_DIRS}) 
target_link_libraries(my_project ${CONAN_LIBS})

set(CMAKE_CXX_FLAGS_DEBUG (-W -Wall -pendantic -std=c++20))

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

add_subdirectory(src)
