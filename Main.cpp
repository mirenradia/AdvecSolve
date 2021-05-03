#include "AdvecSolve_FirstOrderUpwindBox.hpp"
#include <iostream>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " </path/to/parameter file>"
                  << std::endl;
        return 1;
    }

    // construct main solver object
    AdvecSolve::FirstOrderUpwindBox advec_solve;

    // read params from parameter file
    advec_solve.read_params(argv[1]);

    // construct initial grids and set initial data
    advec_solve.set_initial_grids();

    // do the evolution
    advec_solve.run();

    // write animation script which can be passed to gnuplto for visualization
    advec_solve.write_gnuplot_animation_script();

    return 0;
}
