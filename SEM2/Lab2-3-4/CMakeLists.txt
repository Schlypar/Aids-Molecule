cmake_minimum_required(VERSION 3.18.4)

project(Lab2-3-4)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# testing library
find_package(Catch2 REQUIRED)

set(SOURCE_DIR "src")
set(INCLUDE "include")

# setting my files
set(SOURCES
        ${SOURCE_DIR}/main.cpp
)

set(SOURCES_HEADERS
        ${INCLUDE}/ADT.h
        ${INCLUDE}/Pointer.h
        ${INCLUDE}/Iterator.h
        ${INCLUDE}/concepts.h
        ${INCLUDE}/Allocator.h
        ${INCLUDE}/complex.h
        ${INCLUDE}/Logger.h
        ${INCLUDE}/Pair.h
        ${INCLUDE}/Tuple.h
        ${INCLUDE}/str/CString.h
        ${INCLUDE}/str/CString.cpp
        ${INCLUDE}/functional/range.h
        ${INCLUDE}/Sequence/ListSequence.h
        ${INCLUDE}/Sequence/SegmentedList.h
        ${INCLUDE}/Sequence/Sequence.h
        ${INCLUDE}/Sequence/Stack.h
        ${INCLUDE}/Sequence/Array.h
        ${INCLUDE}/Sequence/ArraySequence.h
        ${INCLUDE}/Tree/ITree.h
        ${INCLUDE}/Tree/BinaryTree.h
        ${INCLUDE}/Tree/BinarySearchTree.h
        ${INCLUDE}/Tree/OrderedSet.h
        ${INCLUDE}/Tree/MultiSearchTree.h
        ${INCLUDE}/Tree/BinaryHeap.h
        ${INCLUDE}/Tree/MultiBinaryTree.h
        ${INCLUDE}/Linal/Matrix.h
        ${INCLUDE}/Linal/Vector.h)

add_executable(${PROJECT_NAME} ${SOURCES} ${SOURCES_HEADERS})

target_include_directories(${PROJECT_NAME} PUBLIC ${SOURCE_DIR}/)
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE}/)

# -----------------------------------------------------------------
# python
set (PYTHON_MODULE pointers)

set(PYTHON "/usr/include/python3.11")
find_package (Python3 COMPONENTS Interpreter Development)

add_library(${PYTHON_MODULE} STATIC ${SOURCES_HEADERS})
set_target_properties(${PYTHON_MODULE} PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(${PYTHON_MODULE} PUBLIC ${PYTHON}/)
target_include_directories(${PYTHON_MODULE} PUBLIC ${SOURCE_DIR}/)
target_include_directories(${PYTHON_MODULE} PUBLIC ${INCLUDE}/)

add_subdirectory(pybind11)
pybind11_add_module(smart_pointers python/binder.cpp)
set_target_properties(smart_pointers PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/python/)

target_link_libraries(smart_pointers PRIVATE ${PYTHON_MODULE})
target_link_libraries(smart_pointers PRIVATE pybind11::embed)
target_include_directories(smart_pointers PRIVATE ${PYTHON}/)


# -----------------------------------------------------------------
set(TEST_PROJECT_NAME "tests")

set(TEST_DIR "tests")

# adding tests to my project
add_executable(${TEST_PROJECT_NAME} 
        ${TEST_DIR}/ArrayTests.cpp 
        ${TEST_DIR}/SegmentedListTests.cpp
        ${TEST_DIR}/SequenceTests.cpp
        ${TEST_DIR}/MatrixTests.cpp
        ${TEST_DIR}/BinaryTreeTests.cpp
        ${TEST_DIR}/BinaryHeapTests.cpp
        ${TEST_DIR}/OrderedSetTests.cpp
        ${TEST_DIR}/CStringTests.cpp
        ${SOURCES_HEADERS}
        )

target_include_directories(${TEST_PROJECT_NAME} PRIVATE ${SOURCE_DIR})
target_include_directories(${TEST_PROJECT_NAME} PRIVATE ${INCLUDE})
# target_link_libraries(${TEST_PROJECT_NAME} PRIVATE Catch2::Catch2WithMain)

