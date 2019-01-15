#pragma once

#include <Eigen/Dense>

#include "systemModel.hpp"
#include "constants.hpp"
#include "ecosWrapper.hpp"

using namespace Eigen;

class model_landing_6dof
{
  public:
    model_landing_6dof(){};
    static constexpr size_t n_states = 14;
    static constexpr size_t n_inputs = 3;

    using StateVector = Eigen::Matrix<double, n_states, 1>;
    using ControlVector = Eigen::Matrix<double, n_inputs, 1>;
    using StateMatrix = Eigen::Matrix<double, n_states, n_states>;
    using ControlMatrix = Eigen::Matrix<double, n_states, n_inputs>;

    static string get_name() { return "model_landing_6dof"; }
    static double total_time_guess() { return 8; }

    void initialize(Eigen::Matrix<double, n_states, K> &X,
                    Eigen::Matrix<double, n_inputs, K> &U);

    StateVector ode(const StateVector &x, const ControlVector &u);
    StateMatrix state_jacobian(const StateVector &x, const ControlVector &u);
    ControlMatrix control_jacobian(const StateVector &x, const ControlVector &u);

    void add_application_constraints(optimization_problem::SecondOrderConeProgram &socp,
                                     Eigen::Matrix<double, n_states, K> &X0,
                                     Eigen::Matrix<double, n_inputs, K> &U0);

    static StateVector get_random_state();
    static ControlVector get_random_input();

  private:
    Vector3d g_I = {0, 0, -2};
    Vector3d J_B = {1e-2, 1e-2, 1e-2};
    Vector3d r_T_B = {0, 0, -1e-2};
    double alpha_m = 0.01;
    double T_min = 0.3;
    double T_max = 5.;

    //initial state
    double m_wet = 2.;
    Vector3d r_I_init = {0, 4, 4};
    Vector3d v_I_init = {1, 0, 0};
    Vector4d q_B_I_init = {1., 0., 0., 0.};
    bool constrain_initial_orientation = false;
    Vector3d w_B_init = {0., 0., 0.};

    //final state
    double m_dry = 1.;
    Vector3d r_I_final = {0., 0., 0.};
    Vector3d v_I_final = {0, 0., -1e-1};
    Vector4d q_B_I_final = {1., 0., 0., 0.};
    Vector3d w_B_final = {0., 0., 0.};

    const double tan_delta_max = tan(20. / 180. * M_PI);
    const double cos_theta_max = cos(90. / 180. * M_PI);
    const double tan_gamma_gs = tan(20. / 180. * M_PI);
    const double w_B_max = 60. / 180. * M_PI;

    void Nondimensionalize();
};