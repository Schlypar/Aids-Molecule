# Big note: I am using WLS (Ubuntu distro) so I don't
# know if it will run on windows systems.

# Using package manager called conan (version 1.59.0)

# To build this project we go into build directory (cd build/)
# And then run "cmake -S .. -B . -DCMAKE_BUILD_TYPE=Release" command
# Then "cmake --build ." to actually compile our project
# And finally run our exec file with our case being Lab2-3

# conanfile.txt

# [requires]
# imgui/1.89.4
# glfw/3.3.2
# glew/2.1.0
# glad/0.1.36

# [generators]
# cmake
# CMakeToolchain
# CMakeDeps


# If you are using Unix-like OS you can use launch.sh to configure, compile and run this project for you


cmake_minimum_required(VERSION 3.22.1)

project(Lab2-3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# defining paths to all neccessary *.cmake 
set(opengl_system_DIR "build/")
set(glfw3_DIR "build/")
set(imgui_DIR "build/")
set(xorg_DIR "build/")
set(glad_DIR "build/")

# # did everything as it was said in tutorial
# include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include)
# file(GLOB BUTTERFLIES_SOURCES_C ${CMAKE_CURRENT_SOURCE_DIR} *.c glad/src/gl.c)

# set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/../../../glad/")

# add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)

# glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)

# testing library
find_package(Catch2 REQUIRED)

set(SOURCE_DIR "src")

# setting my files
set(SOURCES
        # ${SOURCE_DIR}/main.cpp
        ${SOURCE_DIR}/test.cpp
        # ${SOURCE_DIR}/GUI.cpp
        # ${SOURCE_DIR}/GUI.h
)

set(SOURCES_HEADERS
        ${SOURCE_DIR}/Matrix.h
        ${SOURCE_DIR}/Allocator.h
        ${SOURCE_DIR}/Array.h
        ${SOURCE_DIR}/ArraySequence.h
        ${SOURCE_DIR}/complex.h
        ${SOURCE_DIR}/ListSequence.h
        ${SOURCE_DIR}/Logger.h
        ${SOURCE_DIR}/Matrix.h
        ${SOURCE_DIR}/SegmentedList.h
        ${SOURCE_DIR}/Sequence.h
        ${SOURCE_DIR}/Tuple.h
        ${SOURCE_DIR}/Vector.h
        ${SOURCE_DIR}/ADT.h)

# find_package(glfw3 REQUIRED)
# find_package(glad REQUIRED)
# find_package(imgui REQUIRED)

# will use later to make script more concise (contains actual relative paths to git cloned repos)
set(IMGUI "../../../imgui")
set(GLFW "../../../glad")
set(GLFW "../../../glfw-3.3.8")

# we will need this as this wont add by itself (headers needed to be included as well)
# set(IMGUI_SRC
#         ${IMGUI}/backends/imgui_impl_opengl3.h
#         ${IMGUI}/backends/imgui_impl_opengl3.cpp
#         ${IMGUI}/backends/imgui_impl_opengl3_loader.h
#         ${IMGUI}/backends/imgui_impl_glfw.h
#         ${IMGUI}/backends/imgui_impl_glfw.cpp
#         ${IMGUI}/imgui.h
#         ${IMGUI}/imgui.cpp
#         ${IMGUI}/imgui_draw.cpp
#         ${IMGUI}/imgui_tables.cpp
#         ${IMGUI}/imgui_widgets.cpp
#         ${IMGUI}/imgui_demo.cpp
# )

# ${IMGUI_SRC}
# compiling our project together including ImGui stuff
add_executable(${PROJECT_NAME} ${SOURCES} ${SOURCES_HEADERS})

# linking our project together
# target_link_libraries(
#   ${PROJECT_NAME}
#     glfw
#     glad_gl_core_33
#     glad::glad
#     imgui::imgui
# )



# adding inlude paths to make sure I can include them in my code
# target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI})
# target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW}/include/)
target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_DIR})

set(TEST_PROJECT_NAME "tests")

set(TEST_DIR "tests")

# set(TESTS_SOURCE 
#         ${TEST_DIR}/moduleTest.cpp 
#         ${TEST_DIR}/ArrayTests.cpp)

# set(TESTS_HEADERS
#         ${TEST_DIR}/ModuleTest.h
#         ${TEST_DIR}/SegmentedListTests.h
#         ${TEST_DIR}/SequenceTests.h)


# adding tests to my project
add_executable(${TEST_PROJECT_NAME} 
        ${TEST_DIR}/ArrayTests.cpp 
        ${TEST_DIR}/SegmentedListTests.cpp
        ${TEST_DIR}/SequenceTests.cpp
        ${TEST_DIR}/MatrixTests.cpp
        ${SOURCES_HEADERS}
        )

target_include_directories(${TEST_PROJECT_NAME} PRIVATE ${SOURCE_DIR})