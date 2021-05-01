cmake_minimum_required(VERSION 3.10)

# Project Name
project(AdvecSolve)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")

# Non-main Source files
file(GLOB SOURCES src/*.cpp)

# Executable
add_executable(advec-solve.ex Main.cpp ${SOURCES})

# Include directories
target_include_directories(advec-solve.ex PUBLIC "${PROJECT_SOURCE_DIR}/src")
