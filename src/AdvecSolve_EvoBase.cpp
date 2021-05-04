#include "AdvecSolve_EvoBase.hpp"
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <sstream>

using namespace AdvecSolve;

EvoBase::EvoBase() {}

double EvoBase::get_time() const { return m_step * m_p.dt_multiplier * m_p.dx; }

void EvoBase::set_grid_coords()
{
    // the location of the cell in (fractional) number of cells
    const double central_cell = 0.5 * m_p.num_cells;

    for (int i = 0; i < m_p.num_cells; ++i)
    {
        // the displacement in units of cells
        double cell_displacement = i - central_cell + 0.5;
        m_grid_coords[i] = m_p.dx * cell_displacement - m_p.centre;
    }
}

template <typename T>
void EvoBase::load_param(const std::string &a_name, T &a_param,
                         std::ifstream &a_params_file_stream)
{
    if (!a_params_file_stream.is_open())
    {
        std::cerr << "Parameter file not open. Cannot read " << a_name << "."
                  << std::endl;
    }

    std::string line;
    std::string search_string = a_name + " =";
    // scan through the whole parameter file
    // this is quite inefficient but since the number of parameters is small
    // it doesn't really matter
    while (a_params_file_stream)
    {
        // get a single line from the parameter file
        std::getline(a_params_file_stream, line);
        if (/* check it's not a comment line*/ line.front() != '#' &&
            /* search for parameter*/ (line.find(search_string) !=
                                       std::string::npos))
        {
            // get substring after =
            std::string param_string = line.substr(search_string.size());
            // convert to stringstream for conversion into a_param
            std::stringstream param_ss(param_string);
            param_ss >> a_param;
            std::cout << a_name << " = " << a_param << "\n";
            return;
        }
    }

    // Since this function returns when the parameter is found this code will
    // executed if the parameter is not found
    std::cerr << "Parameter: " << a_name << " not found in parameter file.\n"
              << "Exiting..." << std::endl;
    exit(3);
}

void EvoBase::write_data() const
{
    const std::string filename = "out.dat";

    // open out file - set mode to append if not at first step
    std::ofstream out_file_stream(filename, (m_step == 0) ? std::ios::out
                                                          : std::ios::app);

    if (!out_file_stream)
    {
        std::cerr << "Error opening " << filename << " for writing at step"
                  << m_step << std::endl;
        // could abort here but there isn't a need
    }

    // set format and write time
    out_file_stream << std::scientific << std::setprecision(10);
    out_file_stream << "# time = " << get_time() << "\n";

    // write data
    for (int i = 0; i < m_p.num_cells; i++)
    {
        out_file_stream << std::setw(20) << m_grid_coords[i] << std::setw(20)
                        << m_state_new[i] << "\n";
    }

    out_file_stream << std::endl;
    // file automatically closes when going out of scope
}

const std::vector<double> &EvoBase::get_data() const { return m_state_new; }

void EvoBase::read_params(const std::filesystem::path &a_params_file_path)
{
    // check parameter file exists
    if (!std::filesystem::exists(a_params_file_path))
    {
        std::cerr << "Parameter file: " << a_params_file_path.string()
                  << " does not exist.\n"
                  << "Exiting..." << std::endl;
        std::exit(1);
    }

    // open parameter file stream
    std::ifstream params_file_stream(a_params_file_path);

    // check that the file could be opened
    if (!params_file_stream)
    {
        std::cerr << "Could not open " << a_params_file_path.string() << "\n"
                  << "Exiting..." << std::endl;
        std::exit(2);
    }

    load_param("num_cells", m_p.num_cells, params_file_stream);
    load_param("max_steps", m_p.max_steps, params_file_stream);
    load_param("write_interval", m_p.write_interval, params_file_stream);
    load_param("advec_speed", m_p.advec_speed, params_file_stream);
    load_param("dt_multiplier", m_p.dt_multiplier, params_file_stream);
    load_param("dx", m_p.dx, params_file_stream);
    load_param("centre", m_p.centre, params_file_stream);
    load_specific_params();

    m_params_set = true;
}

void EvoBase::set_params(const params_t &a_params)
{
    m_p = a_params;
    load_specific_params();
    m_params_set = true;
}

void EvoBase::set_initial_grids()
{
    m_state_new = compute_initial_data();
    m_state_old.resize(m_p.num_cells);
    m_grid_coords.resize(m_p.num_cells);
    set_grid_coords();
}

void EvoBase::run(bool a_write_out)
{
    // check that parameters have been read
    assert(m_params_set);

    // main evolution loop
    for (; m_step < m_p.max_steps; ++m_step)
    {
        // write data to a file if step is a multiple of write_interval
        if (a_write_out && m_step % m_p.write_interval == 0)
        {
            write_data();
        }

        // swap the data from new to old
        m_state_new.swap(m_state_old);
        std::cout << "EvoBase::run: Step: " << std::setw(15) << m_step + 1
                  << "/" << m_p.max_steps << "\r";
        // call the timestep function (implemented in child class)
        timestep();
    }

    // write data at the end
    if (a_write_out && m_step % m_p.write_interval == 0)
    {
        write_data();
    }

    std::cout << std::endl;
}

double EvoBase::compute_error_l2_norm() const
{
    std::vector<double> initial_data = compute_initial_data();

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

void EvoBase::write_gnuplot_animation_script() const
{
    std::string anim_script_filename = "animation.gpi";
    std::ofstream anim_script_stream(anim_script_filename);

    if (!anim_script_stream)
    {
        std::cerr << "Error openining " << anim_script_filename
                  << " for writing." << std::endl;
    }

    anim_script_stream << "set xrange ["
                       << static_cast<int>(std::floor(m_grid_coords.front()))
                       << ":"
                       << static_cast<int>(std::ceil(m_grid_coords.back()))
                       << "]\n";
    anim_script_stream << "do for [i=0:" << m_p.max_steps / m_p.write_interval
                       << "] {\n";
    anim_script_stream << "\ttime = "
                       << m_p.dt_multiplier * m_p.dx * m_p.write_interval
                       << "*i\n";
    anim_script_stream << "\ttitlevar = sprintf(\"time = %f\", time)\n";
    anim_script_stream << "\tplot 'out.dat' using 1:2 every :::i::i with "
                          "lines title titlevar\n";
    anim_script_stream << "\tpause 0.05 \n}\n";
}
