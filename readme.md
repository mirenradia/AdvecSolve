# AdvecSolve

A simple C++ code to solve the 1D advection equation.

## Using the code

### Obtaining the code
Assuming you are already in the directory you want to place the code in,
clone this repository from GitHub in the usual way, for example
```bash
git clone https://github.com/mirenradia/AdvecSolve.git
```

### Prerequisites:
 * [CMake](https://cmake.org/) v3.8 or greater
   * This code has been built successfully with:
     * v3.8.1
     * v3.16.3
     * v3.17.0
 * A C++ compiler with C++17 support
   * This code has been built successfully with:
     * GCC `g++`:
       * v8.4.0
       * v9.3.0
       * v10.2.0
       * v11.1.0
     * Intel C++ Compiler (Classic) `icpc`:
       * v19.0.4.243
     * LLVM Clang `clang++`:
       * v10.0.0
  * Note that older compiler versions
    with partial C++17 support will probably lead to compilation/linking
    errors due to the use of the `std::filesystem` library.

 * _[Optional]_ A non-ancient version of gnuplot for visualizing the output.

### Build instructions

 1. Make a `build` directory and change into it:
 ```bash
 mkdir /path/to/AdvecSolve/build
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

Note that, since the provided initial data is discontinuous and the evolution
scheme used is first order upwind, a significant amount of diffusion is added
to the solution which will lead to large errors for long evolutions.

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

## Design

### Existing implementation

The code is written in a C++ object-oriented style which may seem a little
strange for those familiar with functional programming languages such as C or
Fortran but it should not be too complicated. Rather than calling free
functions, a solver object is created, and then member functions are called
to set up and run the simulation (see [Main.cpp](./Main.cpp)).

All of the code is defined within a namespace:
```cpp
namespace AdvecSolve;
```
in order to avoid any conflicts with other libraries.

There are 2 classes: an abstract base evolution class:
```cpp
class EvoBase;
```
and a specialized child class
```cpp
class FirstOrderUpwindBox;
```
which inherits from `EvoBase`.

`EvoBase` is intended to be agnostic of the specific evolution scheme and
initial data. It provides/implements:
 * Parameter reading and storage (`m_p`)
 * State vector storage (`m_state_new` and `m_state_old`)
 * Data output (`write_data()`)
 * Error norm calculation - since we know the analytic solution to the advection
   equation (`compute_error_l2_norm()`)
 * Set up of initial grids (`set_initial_grids()`)
 * The outer part of the main evolution loop in `run()` which calls `timestep()`
   (defined in the child class) at each timestep

`FirstOrderUpwindBox` specialises `EvoBase` for a specific problem and evolution
scheme. It implements:
 * The `timestep()` function called by `EvoBase::run()` which implements the
   [first-order upwind evolution scheme](
   https://en.wikipedia.org/wiki/Upwind_scheme)
 * The computation of the initial data (`initial_data()`)

### Modifying the code

If you wish to use the existing evolution scheme but just change the initial
data, you can define a new child class of `FirstOrderUpwindBox` which overrides
the virtual function `initial_data()` (for example, see [ConvergenceTest.cpp](
./ConvergenceTest.cpp)).

If you wish to use a different evolution scheme, it may be easier to write your
own child class of `EvoBase` and implement its virtual functions as in
`FirstOrderUpwindBox`.

For more substantial changes, it is probably easier to read the code in order
to understand how it works
