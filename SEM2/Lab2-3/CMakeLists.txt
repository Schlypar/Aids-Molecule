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



# setting my files
set(SOURCES
        # src/main.cpp
        src/test.cpp
        src/moduleTest.cpp
        # src/GUI.cpp
        # src/GUI.h
)

# find_package(glfw3 REQUIRED)
# find_package(glad REQUIRED)
# find_package(imgui REQUIRED)

# will use later to make script more concise (contains actual relative paths to git cloned repos)
set(IMGUI "../../../imgui")
set(GLFW "../../../glad")
set(GLFW "../../../glfw-3.3.8")

# we will need this as this wont add by itself (headers needed to be included as well)
set(IMGUI_SRC
        ${IMGUI}/backends/imgui_impl_opengl3.h
        ${IMGUI}/backends/imgui_impl_opengl3.cpp
        ${IMGUI}/backends/imgui_impl_opengl3_loader.h
        ${IMGUI}/backends/imgui_impl_glfw.h
        ${IMGUI}/backends/imgui_impl_glfw.cpp
        ${IMGUI}/imgui.h
        ${IMGUI}/imgui.cpp
        ${IMGUI}/imgui_draw.cpp
        ${IMGUI}/imgui_tables.cpp
        ${IMGUI}/imgui_widgets.cpp
        ${IMGUI}/imgui_demo.cpp
)

# ${IMGUI_SRC}
# compiling our project together including ImGui stuff
add_executable(${PROJECT_NAME} ${SOURCES})

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