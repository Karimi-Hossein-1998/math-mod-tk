#include "../includes/solvers/DDE/rk/explicit/rk4-dde-variants.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk4-dde-general-variants.hpp"

int main() 
{
    std::cout << "=== RK4 Gill DDE Solver Test ===" << std::endl;
    
    // Test 1: Single delay
    std::cout << "\nTest 1: Single Delay DDE" << std::endl;
    std::cout << "Problem: dy/dt = -y(t) + 0.5*y(t - 1.0)" << std::endl;
    
    DelayFunc f_single = [](double t, const dVec& y, const dVec& y_delayed) 
    {
        dVec  dydt(y.size());
        for (size_t i = 0; i < y.size(); ++i)
        {
            dydt[i] = -y[i] + 0.5 * y_delayed[i];
        }
        return dydt;
    };
    
    Func history = [](double t) 
    {
        return dVec {1.0};
    };
    
    SolverParameters params_single;
    params_single.delay_derivative = f_single;
    params_single.initial_conditions = {1.0};
    params_single.t0 = 0.0;
    params_single.t1 = 5.0;
    params_single.dt = 0.1;
    params_single.delay_times = {1.0};
    params_single.history_function = history;
    params_single.is_dde = true;
    
    SolverResults results_single = rk4_gill_dde_solver(params_single);
    
    std::cout << "Solution at t = 5.0: " << std::fixed << std::setprecision(6) 
              << results_single.solution.back()[0] << std::endl;
    
    // Test 2: Multiple delays
    std::cout << "\nTest 2: Multiple Delays DDE" << std::endl;
    std::cout << "Problem: dy/dt = -y(t) + 0.3*y(t - 0.5) + 0.2*y(t - 1.0)" << std::endl;
    
    DelayFuncMulti f_multi = [](double t, const dVec& y, const dMatrix& y_delayed_states) 
    {
        dVec  dydt(y.size());
        for (size_t i = 0; i < y.size(); ++i)
        {
            dydt[i] = -y[i] + 0.3 * y_delayed_states[0][i] + 0.2 * y_delayed_states[1][i];
        }
        return dydt;
    };
    
    SolverParameters params_multi;
    params_multi.delay_derivative_multi = f_multi;
    params_multi.initial_conditions = {1.0};
    params_multi.t0 = 0.0;
    params_multi.t1 = 5.0;
    params_multi.dt = 0.1;
    params_multi.delay_times = {0.5, 1.0};
    params_multi.history_function = history;
    params_multi.is_dde = true;
    
    SolverResults results_multi = rk4_gill_dde_general_solver(params_multi);
    
    std::cout << "Solution at t = 5.0: " << std::fixed << std::setprecision(6) 
              << results_multi.solution.back()[0] << std::endl;
    
    // Test 3: Exponential decay with delay
    std::cout << "\nTest 3: Exponential Decay with Delay" << std::endl;
    std::cout << "Problem: dy/dt = -2*y(t) + exp(-y(t - 0.3))" << std::endl;
    
    DelayFunc f_exp = [](double t, const dVec& y, const dVec& y_delayed) 
    {
        dVec  dydt(y.size());
        for (size_t i = 0; i < y.size(); ++i)
        {
            dydt[i] = -2.0 * y[i] + std::exp(-y_delayed[i]);
        }
        return dydt;
    };
    
    Func history_exp = [](double t) 
    {
        return dVec {1.0};
    };
    
    SolverParameters params_exp;
    params_exp.delay_derivative = f_exp;
    params_exp.initial_conditions = {1.0};
    params_exp.t0 = 0.0;
    params_exp.t1 = 4.0;
    params_exp.dt = 0.05;
    params_exp.delay_times = {0.3};
    params_exp.history_function = history_exp;
    params_exp.is_dde = true;
    
    SolverResults results_exp = rk4_gill_dde_solver(params_exp);
    
    std::cout << "Solution at t = 4.0: " << std::fixed << std::setprecision(6) 
              << results_exp.solution.back()[0] << std::endl;
    
    // Test 4: Complex system of DDEs with multiple delays
    std::cout << "\nTest 4: Complex System of DDEs with Multiple Delays" << std::endl;
    std::cout << "Problem: dx/dt = -x(t) + 0.7*y(t - 1.2) + 0.1*z(t - 0.8)" << std::endl;
    std::cout << "         dy/dt = -y(t) + 0.5*x(t - 0.6) + 0.3*z(t - 1.0)" << std::endl;
    std::cout << "         dz/dt = -z(t) + 0.4*x(t - 0.4) + 0.2*y(t - 1.4)" << std::endl;
    
    DelayFuncMulti f_complex = [](double t, const dVec& y, const dMatrix& y_delayed_states) 
    {
        dVec  dydt(y.size());
        // dx/dt = -x(t) + 0.7*y(t - 1.2) + 0.1*z(t - 0.8)
        dydt[0] = -y[0] + 0.7 * y_delayed_states[0][1] + 0.1 * y_delayed_states[1][2];
        // dy/dt = -y(t) + 0.5*x(t - 0.6) + 0.3*z(t - 1.0)
        dydt[1] = -y[1] + 0.5 * y_delayed_states[2][0] + 0.3 * y_delayed_states[3][2];
        // dz/dt = -z(t) + 0.4*x(t - 0.4) + 0.2*y(t - 1.4)
        dydt[2] = -y[2] + 0.4 * y_delayed_states[4][0] + 0.2 * y_delayed_states[5][1];
        return dydt;
    };
    
    Func history_complex = [](double t) 
    {
        return dVec {1.0, 0.8, 0.6};
    };
    
    SolverParameters params_complex;
    params_complex.delay_derivative_multi = f_complex;
    params_complex.initial_conditions = {1.0, 0.8, 0.6};
    params_complex.t0 = 0.0;
    params_complex.t1 = 10.0;
    params_complex.dt = 0.1;
    params_complex.delay_times = {1.2, 0.8, 0.6, 1.0, 0.4, 1.4};  // Order matters for indexing
    params_complex.history_function = history_complex;
    params_complex.is_dde = true;
    
    SolverResults results_complex = rk4_gill_dde_general_solver(params_complex);
    
    std::cout << "Solution at t = 10.0:" << std::endl;
    std::cout << "  x = " << std::fixed << std::setprecision(6) << results_complex.solution.back()[0] << std::endl;
    std::cout << "  y = " << std::fixed << std::setprecision(6) << results_complex.solution.back()[1] << std::endl;
    std::cout << "  z = " << std::fixed << std::setprecision(6) << results_complex.solution.back()[2] << std::endl;
    
    std::cout << "\nRK4 Gill DDE solvers test completed successfully!" << std::endl;
    
    return 0;
} 