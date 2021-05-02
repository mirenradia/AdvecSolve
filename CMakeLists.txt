cmake_minimum_required(VERSION 3.10)

# Project Name
project(AdvecSolve)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")

# Non-main Source files
file(GLOB SOURCES src/*.cpp)

# Main executable
add_executable(advec-solve.ex Main.cpp ${SOURCES})

# Test executable
add_executable(convergence-test.ex ConvergenceTest.cpp ${SOURCES})

# Include directories for both executables
include_directories("${PROJECT_SOURCE_DIR}/src")
