# Big note: I am using WLS (Ubuntu distro) so I don't
# know if it will run on windows systems.

# Using package manager called Conan (version 1.59.0)

# Conan does almost all the work but still I cannot do this cleanly
# But there is still some libs outside of this project scope
# So that you can see ../../../ 


cmake_minimum_required(VERSION 3.22.1)

project(Lab2)

# defining paths to all neccessary *.cmake 
set(opengl_system_DIR "build/")
set(glfw3_DIR "build/")
set(imgui_DIR "build/")
set(xorg_DIR "build/")

# did everything as it was said in tutorial
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

# setting dependency on ImGui library
set(IMGUI "../../../imgui")
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

# compiling a project
add_executable(${PROJECT_NAME} ${SOURCES} ${IMGUI_SRC})

# adding inlude paths to make sure I can include them in my code
target_include_directories(${PROJECT_NAME} PRIVATE ${IMGUI})
include_directories(../../../glfw-3.3.8/GLFW/include/GLFW/)

# using conan to find installed packages
find_package(glfw3 REQUIRED)
find_package(OpenGL REQUIRED)

# linking itself
target_link_libraries(
    ${PROJECT_NAME}
    glad_gl_core_33
    glfw
)