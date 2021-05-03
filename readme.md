# AdvecSolve

A simple C++ code to solve the advection equation.

### Obtaining the code
Clone this repository from GitHub in the usual way, for example
```bash
git clone https://github.com/mirenradia/AdvecSolve.git
```

### Prerequisites:
 * CMake v3.10 or greater (tested: v3.16.3)
 * A C++ compiler with C++17 support e.g. g++ v8 or greater
   (tested: g++ v8.4.0, v9.3.0, v10.2.0 and v11.1.0). Note that older versions
   with partial C++17 support will probably lead to compilation errors due to
   the lack of the `std::filesystem` library.
 * _[Optional]_ A non-ancient version of gnuplot for visualizing the output
   (tested: gnuplot v5.2.8)

### Build Instructions

 1. Change directory into the `build` subdirectory:
 ```bash
 cd /path/to/AdvecSolve/build
 ```

 2. Generate CMake configuration files:
 ```bash
 cmake ..
 ```

 3. Build using cmake:
 ```bash
 cmake --build .
 ```
 Two executables will be created: `advec-solve.ex` and `convergence-test.ex`
 in the `build` directory.

### Running the test

This test checks the convergence of the first order upwind evolution using
initial data of a simple cosine function (since convergence can only be
reliably checked with continuous initial data). To run it, simply execute the
binary with no arguments:
```bash
./convergence-test.ex
```
If successful, the final printed line will say
```
Convergence Test PASSED
```

### Running the code

The parameters of the code are set by a simple parameter file:
[params.txt](./params.txt). The parameters should be self-explanatory but there
are explanatory comments if not. To run the code, pass this as the first and
only argument to the binary, for example, in the `build` directory, you would
do:
```bash
./advec_solve.ex ../params.txt
```

### Code output

After running, the code will write a single ASCII data file named
`out.dat` in the current directory. This contains the state vector at
regular intervals (controlled by the `write_interval` parameter). There will
also be an animation script named `animation.gpi` in the current directory
which can be used to visualise the data using
[`gnuplot`](http://www.gnuplot.info/):
```bash
gnuplot animation.gpi
```
