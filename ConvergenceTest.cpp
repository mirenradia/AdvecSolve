#include "AdvecSolve_FirstOrderUpwindBox.hpp"
#include <cmath>
#include <iomanip>
#include <iostream>

namespace AdvecSolve
{
// Since the existing initial data in UpwindBox is discontinuous, redefine
// the initial data to something continuous in order to test the convergence
// of the evolution
class FirstOrderUpwindCos : public FirstOrderUpwindBox
{
    virtual std::vector<double> compute_initial_data() const override
    {
        std::vector<double> out(m_p.num_cells);
        for (int i = 0; i < m_p.num_cells; ++i)
        {
            out[i] = std::cos(2.0 * M_PI * i / m_p.num_cells);
        }
        return out;
    }
};

} // namespace AdvecSolve

int main(int argc, char *argv[])
{
    using namespace AdvecSolve;

    // hardcoded parameters for the coarse evolution
    params_t coarse_params;
    coarse_params.num_cells = 1000;
    coarse_params.max_steps = 1000;
    coarse_params.write_interval = 50; // irrelevant as we won't write out
    coarse_params.advec_speed = 1.0;
    coarse_params.dt_multiplier = 0.1;
    coarse_params.dx = 1.0;
    coarse_params.centre = 0.0;

    // parameters for the fine evolution copy from coarse params and change
    // below as required
    params_t fine_params = coarse_params;
    fine_params.num_cells *= 2;
    fine_params.max_steps *= 2;
    fine_params.dx *= 0.5;

    // construct simulation objects for each evolution
    FirstOrderUpwindCos advec_solve_coarse;
    FirstOrderUpwindCos advec_solve_fine;

    // set parameters
    advec_solve_coarse.set_params(coarse_params);
    advec_solve_fine.set_params(fine_params);

    // construct initial grids and set initial data
    advec_solve_coarse.set_initial_grids();
    advec_solve_fine.set_initial_grids();

    // Do the evolution
    bool write_out = false; // we don't want output
    std::cout << "Evolving Coarse..."
              << "\n";
    advec_solve_coarse.run(write_out);
    std::cout << "Evolving fine..."
              << "\n";
    advec_solve_fine.run(write_out);

    std::cout << std::setprecision(16);

    double coarse_error = advec_solve_coarse.compute_error_l2_norm();
    std::cout << "coarse_error = " << coarse_error << "\n";
    double fine_error = advec_solve_fine.compute_error_l2_norm();
    std::cout << "fine_error = " << fine_error << "\n";

    double convergence_ratio = coarse_error / fine_error;
    std::cout << "convergence_ratio = " << convergence_ratio << "\n";

    // Since the upwind method is first order, we expect the convergence
    // ratio to be 2 but allow a 5% tolerance
    if (std::abs(convergence_ratio - 2.0) < 0.1)
    {
        std::cout << "Convergence Test PASSED" << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Convergence Test FAILED" << std::endl;
        return 1;
    }
}
