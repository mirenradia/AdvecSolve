cmake_minimum_required(VERSION 3.8)

# Project Name
project(AdvecSolve)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")

# Non-main Source files to be built into libadvecsolve
file(GLOB SOURCES src/*.cpp)

# advecsolve library - to avoid compiling files twice for both the main code and
# test
add_library(advecsolve ${SOURCES})

# Link libraries for both executables. Note libstd++fs is for some older GCC
# versions as this is part of libstdc++ for newer GCC versions
link_libraries(advecsolve stdc++fs)

# Main executable
add_executable(advec-solve.ex Main.cpp)

# Test executable
add_executable(convergence-test.ex ConvergenceTest.cpp)

# Include directories for both executables
include_directories("${PROJECT_SOURCE_DIR}/src")
