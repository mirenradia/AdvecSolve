#ifndef ADVECSOLVE_UPWINDBOX_HPP_
#define ADVECSOLVE_UPWINDBOX_HPP_

#include "AdvecSolve_EvoBase.hpp"

// This is the main project namespace
namespace AdvecSolve
{

class UpwindBox : public EvoBase
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
    // Sets the box initial data
    virtual void set_initial_data() override;

    // Use the analytic solution to calculate the L2 norm of the error
    double compute_error_l2_norm() const;
};

} // namespace AdvecSolve
#endif /* ADVECSOLVE_UPWINDBOX_HPP_ */
