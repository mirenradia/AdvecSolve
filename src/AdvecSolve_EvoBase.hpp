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
    // just puts the x-coordinates of the grid in m_grid_coords
    // keep this private as no child class should need this
    void set_grid_coords();

  protected:
    // the parameter struct
    params_t m_p;
    // check whether parameters have been read
    bool m_params_set = false;
    // the solution now
    std::vector<double> m_state_new;
    // the solution at the previous timestep
    std::vector<double> m_state_old;
    // the coordinates on the grid
    std::vector<double> m_grid_coords;
    // the current timestep
    int m_step = 0;

    // load a single parameter from the parameter file
    template <typename T>
    void load_param(const std::string &a_name, T &a_param,
                    std::ifstream &a_params_file_stream);

    // load specific parameters (pure virtual)
    virtual void load_specific_params() = 0;

    // advance one timestep (pure virtual)
    virtual void timestep() = 0;

    // write data (in m_state_new) to a file in ASCII format
    void write_data() const;

  public:
    // constructor
    EvoBase();

    // returns the current time
    double get_time() const;

    // returns m_state_new
    const std::vector<double> &get_data() const;

    // read parameters
    void read_params(const std::filesystem::path &a_params_file_path);

    // set parameters
    void set_params(const params_t &a_params);

    // set initial data
    void set_initial_grids();

    // construct initial data (pure virtual)
    virtual std::vector<double> compute_initial_data() const = 0;

    // do the evolution!
    void run(bool a_write_out = true);

    // use the analytic solution to compute the L2 norm of the error
    double compute_error_l2_norm() const;

    // writes a script that gnuplot can use to visualise the data
    void write_gnuplot_animation_script() const;
};

} // namespace AdvecSolve
#endif /* ADVECSOLVE_EVOBASE_HPP_ */
