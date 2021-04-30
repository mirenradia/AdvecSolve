cmake_minimum_required(VERSION 3.10)

# Project Name
project(AdvecSolve)

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Library
add_library(AdvecSolveBase src/AdvecSolveBase.cpp)

# Executable
add_executable(advec-solve.ex Main.cpp)
target_link_libraries(advec-solve.ex AdvecSolveBase)

# Include directories
target_include_directories(advec-solve.ex PUBLIC "${PROJECT_SOURCE_DIR}/src")
