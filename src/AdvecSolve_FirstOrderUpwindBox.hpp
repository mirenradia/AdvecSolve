#ifndef ADVECSOLVE_FIRSTORDERUPWINDBOX_HPP_
#define ADVECSOLVE_FIRSTORDERUPWINDBOX_HPP_

#include "AdvecSolve_EvoBase.hpp"

// This is the main project namespace
namespace AdvecSolve
{

// This is a child class of EvoBase and implements the specific evolution
// method and initial data
class FirstOrderUpwindBox : public EvoBase
{
  private:
    // Since the stencil changes depending on the sign of the speed
    // store the sign in this bool to avoid calculating at every step
    bool m_advec_speed_positive;

  protected:
    // Implements the evolution by 1 step using the upwind method
    virtual void timestep() override;

    // Loads the parameters specific to this class
    virtual void load_specific_params() override;

  public:
    // Computes the box-like initial data
    virtual std::vector<double> compute_initial_data() const override;
};

} // namespace AdvecSolve
#endif /* ADVECSOLVE_FIRSTORDERUPWINDBOX_HPP_ */
