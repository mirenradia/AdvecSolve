#ifndef ADVECSOLVE_EVOBASE_HPP_
#define ADVECSOLVE_EVOBASE_HPP_

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// This is the main project namespace
namespace AdvecSolve
{

// The parameter struct
struct params_t
{
    int num_cells;        // commonly denoted "n"
    int max_steps;        // how many steps to evolve for
    int write_interval;   // how frequently to write data to a file
    double advec_speed;   // denoted a
    double dt_multiplier; // Courant factor = dt/dx
    double dx;            // grid spacing
    double centre;        // the x-coordinate of the centre
};

// A base class for doing the time evolution. It is necessary to
// define a child class to implement the specific evolution and initial data.
class EvoBase
{
  private:
    void set_grid_coords();

  protected:
    // the parameter struct
    params_t m_p;
    // check whether parameters have been read
    bool m_params_read = false;
    // the solution now
    std::vector<double> m_state_new;
    // the solution at the previous timestep
    std::vector<double> m_state_old;
    // the coordinates on the grid
    std::vector<double> m_grid_coords;
    // the current timestep
    int m_step = 0;

    // Load a single parameter from the parameter file
    template <typename T>
    void load_param(const std::string &a_name, T &a_param,
                    std::ifstream &a_params_file_stream);

    // Load specific parameters (pure virtual)
    virtual void load_specific_params() = 0;

    // advance one timestep (pure virtual)
    virtual void timestep() = 0;

    // Write data (in m_state_new) to a file in ASCII format
    void write_data();

  public:
    // constructor
    EvoBase();

    // returns the current time
    double get_time();

    // read parameters
    void read_params(const std::filesystem::path &a_params_file_path);

    // set initial data (pure virtual)
    virtual void set_initial_data() = 0;

    // do the evolution!
    void run();
};

} // namespace AdvecSolve
#endif /* ADVECSOLVE_EVOBASE_HPP_ */
