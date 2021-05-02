#include "AdvecSolve_UpwindBox.hpp"
#include <cmath>

using namespace AdvecSolve;

void UpwindBox::timestep()
{
    // references for aesthetic reasons only
    std::vector<double> &u_new = m_state_new;
    std::vector<double> &u_old = m_state_old;

    if (m_advec_speed_positive)
    {
        // since we use periodic boundary conditions u[-1] = u[n - 1]
        u_new[0] = u_old[0] - m_p.advec_speed * m_p.dt_multiplier *
                                  (u_old[0] - u_old[m_p.num_cells - 1]);

        // loop over grid
        for (int i = 1; i < m_p.num_cells; ++i)
        {
            u_new[i] = u_old[i] - m_p.advec_speed * m_p.dt_multiplier *
                                      (u_old[i] - u_old[i - 1]);
        }
    }
    else
    {
        // since we use periodic boundary conditions u[-1] = u[n - 1]
        u_new[m_p.num_cells - 1] = u_old[m_p.num_cells - 1] -
                                   m_p.advec_speed * m_p.dt_multiplier *
                                       (u_old[0] - u_old[m_p.num_cells - 1]);

        // loop over grid
        for (int i = 0; i < m_p.num_cells - 1; ++i)
        {
            u_new[i] = u_old[i] - m_p.advec_speed * m_p.dt_multiplier *
                                      (u_old[i + 1] - u_old[i]);
        }
    }
}

void UpwindBox::load_specific_params()
{
    m_advec_speed_positive = (m_p.advec_speed > 0.0);
}

void UpwindBox::set_initial_data()
{
    for (int i = 0; i < m_p.num_cells; ++i)
    {
        if (i + 1 < std::round(0.25 * m_p.num_cells) ||
            i + 1 > std::round(0.75 * m_p.num_cells))
            m_state_new[i] = 0.1;
        else
            m_state_new[i] = 1.0;
    }
}

double UpwindBox::compute_error_l2_norm() const
{
    std::vector<double> initial_data(m_p.num_cells);
    for (int i = 0; i < m_p.num_cells; ++i)
    {
        if (i + 1 < std::round(0.25 * m_p.num_cells) ||
            i + 1 > std::round(0.75 * m_p.num_cells))
            initial_data[i] = 0.1;
        else
            initial_data[i] = 1.0;
    }

    int analytic_cell_shift = std::round(get_time() * m_p.advec_speed / m_p.dx);

    std::vector<double> analytic_solution(m_p.num_cells);
    for (int i = 0; i < m_p.num_cells; ++i)
    {
        int initial_data_idx = (i - analytic_cell_shift);
        // deal with periodicity below
        while (initial_data_idx < 0)
        {
            initial_data_idx += m_p.num_cells;
        }
        while (initial_data_idx >= m_p.num_cells)
        {
            initial_data_idx -= m_p.num_cells;
        }
        analytic_solution[i] = initial_data[initial_data_idx];
    }

    double norm_sq = 0.0;
    for (int i = 0; i < m_p.num_cells; ++i)
    {
        double error = m_state_new[i] - analytic_solution[i];
        norm_sq += error * error;
    }
    // need to normalise by grid spacing
    norm_sq *= m_p.dx;

    return std::sqrt(norm_sq);
}
