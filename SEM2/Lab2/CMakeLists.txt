cmake_minimum_required(VERSION 3.22.1)

project(Lab2)

set(SOURCES src/main.cpp src/moduleTest.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

add_