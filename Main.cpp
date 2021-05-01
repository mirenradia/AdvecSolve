#include "AdvecSolveBase.hpp"

int main(int argc, char *argv[])
{
    AdvecSolveBase advec_solve_base;

    if (argc >= 2)
        advec_solve_base.read_params(argv[1]);
    return 0;
}
