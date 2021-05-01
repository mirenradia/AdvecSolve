#ifndef ADVECSOLVE_UPWINDBOX_HPP_
#define ADVECSOLVE_UPWINDBOX_HPP_

#include "AdvecSolve_EvoBase.hpp"

// This is the main project namespace
namespace AdvecSolve
{

class UpwindBox : public EvoBase
{
  protected:
    // Implements the evolution by 1 step using the upwind method
    virtual void timestep() override;

    // Loads the parameters specific to this class
    virtual void load_specific_params() override;

  public:
    // Sets the box initial data
    virtual void set_initial_data() override;
};

} // namespace AdvecSolve
#endif /* ADVECSOLVE_UPWINDBOX_HPP_ */
