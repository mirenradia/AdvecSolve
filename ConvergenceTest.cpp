#include "AdvecSolve_UpwindBox.hpp"

int main(int argc, char *argv[])
{
    using namespace AdvecSolve;

    // parameters for the coarse evolution
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

    // Construct simulation objects for each evolution
    UpwindBox advec_solve_coarse;
    UpwindBox advec_solve_fine;

    // Set parameters
    advec_solve_coarse.set_params(coarse_params);
    advec_solve_fine.set_params(fine_params);

    // Set the initial data
    advec_solve_coarse.set_initial_data();
    advec_solve_fine.set_initial_data();

    // Do the evolution
    bool write_out = false; // we don't want output
    std::cout << "Evolving Coarse..."
              << "\n";
    advec_solve_coarse.run(write_out);
    std::cout << "Evolving fine..."
              << "\n";
    advec_solve_fine.run(write_out);

    double coarse_error = advec_solve_coarse.compute_error_l2_norm();
    std::cout << "coarse_error = " << std::setprecision(16) << coarse_error
              << "\n";
    double fine_error = advec_solve_fine.compute_error_l2_norm();
    std::cout << "fine_error = " << std::setprecision(16) << fine_error << "\n";

    return 0;
}
