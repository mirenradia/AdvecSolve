#include "AdvecSolve_UpwindBox.hpp"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: " << argv[0] << " </path/to/parameter file>"
                  << std::endl;
        return 1;
    }

    // Construct main solver object
    AdvecSolve::UpwindBox advec_solve;

    // read params from parameter file
    advec_solve.read_params(argv[1]);

    // set the initial data
    advec_solve.set_initial_data();

    // do the evolution
    advec_solve.run();

    return 0;
}
