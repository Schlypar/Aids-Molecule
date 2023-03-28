# Big note: I am using WLS (Ubuntu distro) so I don't
# know if it will run on windows systems.

# Using package manager called vckpg

# To build this project we go into build directory
# And then run "cmake -S .. -B . -DCMAKE_BUILD_TYPE=Release" command or "cmake -S .. -B . -DCMAKE_TOOLCHAIN_FILE=$PWD/vcpkg/scripts/buildsystems/vcpkg.cmake" to debug (did not checked thiss yet)
# Then "cmake --build ." to actually compile our project
# And finally run our exec file with our case being Lab2

# If you are using Unix-like OS you can use launch.sh to configure, compile and run this project for you

# To install dependencies you need to create file named vckpg.json and fill it with

# {
#   "name": "Lab2",
#   "version": "0.1.0",
#   "dependencies": [
#     "glfw3",
#     "glad",
#     {
#       "name": "imgui",
#       "features": ["glfw-binding", "opengl3-glad-binding"]
#     }
#   ]
# }

cmake_minimum_required(VERSION 3.22.1)

project(Lab2)

# defining paths to all neccessary *.cmake 
set(opengl_system_DIR "build/")
set(glfw3_DIR "build/")
set(imgui_DIR "build/")
set(xorg_DIR "build/")
set(glad_DIR "build/")

# # did everything as it was said in tutorial
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/external/glad/include)
file(GLOB BUTTERFLIES_SOURCES_C ${CMAKE_CURRENT_SOURCE_DIR} *.c glad/src/gl.c)

set(GLAD_SOURCES_DIR "${PROJECT_SOURCE_DIR}/../../../glad/")

add_subdirectory("${GLAD_SOURCES_DIR}/cmake" glad_cmake)

glad_add_library(glad_gl_core_33 REPRODUCIBLE API gl:core=3.3)



# setting my files
set(SOURCES
        src/main.cpp
        src/moduleTest.cpp 
)

find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(imgui REQUIRED)

# will use later to make script more concise (contains actual relative paths to git cloned repos)
set(IMGUI "../../../imgui")
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

# compiling our project together including ImGui stuff
add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SRC})

# linking our project together
target_link_libraries(
  ${PROJECT_NAME}
    glfw
    glad_gl_core_33
    glad::glad
    imgui::imgui
)

# adding inlude paths to make sure I can include them in my code
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI})
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW}/include/)