# AdvecSolve

A simple C++ code to solve the advection equation.

### Prerequisites:
 * CMake v3.10 or greater (tested: v3.16.3)
 * A C++ compiler with C++17 support e.g. g++ v8 or greater (tested: g++ v10.2.0)

### Build Instructions

 1. Change directory into the `build` subdirectory:
 ```bash
 cd build
 ```

 2. Generate CMake configuration files:
 ```bash
 cmake ..
 ```

 3. Build using cmake:
 ```bash
 cmake --build .
 ```

An executable called advec-solve.ex will be created in the `build` directory
which can be run in the usual way
```bash
./advec_solve.ex
```
