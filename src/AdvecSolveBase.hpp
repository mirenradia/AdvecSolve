#ifndef ADVECSOLVEBASE_HPP_
#define ADVECSOLVEBASE_HPP_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class AdvecSolveBase
{
  public:
    struct params_t
    {
        int num_cells;        // commonly denoted "n"
        int max_steps;        // how many steps to evolve for
        double wave_speed;    // denoted a
        double dt_multiplier; // Courant factor = dt/dx
        double dx;            // grid spacing
        double origin;        // the x-coordinate of the origin
    };

  protected:
    // the parameter struct
    params_t m_p;
    // the solution now
    std::vector<double> m_state_new;
    // the solution at the previous timestep
    std::vector<double> m_state_old;
    // the current timestep
    int m_step = 0;

    // Load a single parameter from the parameter file
    template <typename T>
    void load_param(const std::string &a_name, T &a_param,
                    std::ifstream &a_params_file_stream);

  public:
    // constructor
    AdvecSolveBase();

    // read parameters
    void read_params(const std::filesystem::path &a_params_file_path);
};

#endif /* ADVECSOLVEBASE_HPP_ */
