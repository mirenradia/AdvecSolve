#include "AdvecSolveBase.hpp"
#include <cstdlib>
#include <sstream>

AdvecSolveBase::AdvecSolveBase() {}

template <typename T>
void AdvecSolveBase::load_param(const std::string &a_name, T &a_param,
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
    while (a_params_file_stream)
    {
        // get a single line from the parameter file
        std::getline(a_params_file_stream, line);
        if (/* check it's not a comment line*/ line.front() != '#' &&
            /* search for parameter*/ (line.find(search_string) !=
                                       std::string::npos))
        {
            std::string param_string = line.substr(search_string.size());
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

void AdvecSolveBase::read_params(
    const std::filesystem::path &a_params_file_path)
{
    // check params file exists
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
    m_state_new.resize(m_p.num_cells);
    m_state_old.resize(m_p.num_cells);
    load_param("max_steps", m_p.max_steps, params_file_stream);
    load_param("wave_speed", m_p.wave_speed, params_file_stream);
    load_param("dt_multiplier", m_p.dt_multiplier, params_file_stream);
    load_param("dx", m_p.dx, params_file_stream);
    load_param("origin", m_p.origin, params_file_stream);
}
