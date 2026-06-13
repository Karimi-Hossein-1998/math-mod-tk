#include "../includes/solvers/DDE/rk/explicit/rk4-dde-variants.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk4-dde-general-variants.hpp"

int main() 
{
    std::cout << "=== RK4 Ralston DDE Solver Test ===" << std::endl;
    
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
    
    SolverResults results_single = rk4_ralston_dde_solver(params_single);
    
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
    
    SolverResults results_multi = rk4_ralston_dde_general_solver(params_multi);
    
    std::cout << "Solution at t = 5.0: " << std::fixed << std::setprecision(6) 
              << results_multi.solution.back()[0] << std::endl;
    
    // Test 3: Nonlinear DDE with single delay
    std::cout << "\nTest 3: Nonlinear DDE with Single Delay" << std::endl;
    std::cout << "Problem: dy/dt = -y(t)^2 + 0.8*y(t - 0.5)" << std::endl;
    
    DelayFunc f_nonlinear = [](double t, const dVec& y, const dVec& y_delayed) 
    {
        dVec  dydt(y.size());
        for (size_t i = 0; i < y.size(); ++i)
        {
            dydt[i] = -y[i] * y[i] + 0.8 * y_delayed[i];
        }
        return dydt;
    };
    
    Func history_nonlinear = [](double t) 
    {
        return dVec {0.8};
    };
    
    SolverParameters params_nonlinear;
    params_nonlinear.delay_derivative = f_nonlinear;
    params_nonlinear.initial_conditions = {0.8};
    params_nonlinear.t0 = 0.0;
    params_nonlinear.t1 = 6.0;
    params_nonlinear.dt = 0.05;
    params_nonlinear.delay_times = {0.5};
    params_nonlinear.history_function = history_nonlinear;
    params_nonlinear.is_dde = true;
    
    SolverResults results_nonlinear = rk4_ralston_dde_solver(params_nonlinear);
    
    std::cout << "Solution at t = 6.0: " << std::fixed << std::setprecision(6) 
              << results_nonlinear.solution.back()[0] << std::endl;
    
    // Test 4: Coupled system of DDEs with multiple delays
    std::cout << "\nTest 4: Coupled System of DDEs with Multiple Delays" << std::endl;
    std::cout << "Problem: dx/dt = -x(t) + 0.6*y(t - 1.0)" << std::endl;
    std::cout << "         dy/dt = -y(t) + 0.4*x(t - 0.5) + 0.3*y(t - 1.5)" << std::endl;
    
    DelayFuncMulti f_coupled = [](double t, const dVec& y, const dMatrix& y_delayed_states) 
    {
        dVec  dydt(y.size());
        // dx/dt = -x(t) + 0.6*y(t - 1.0)
        dydt[0] = -y[0] + 0.6 * y_delayed_states[0][1];  // y_delayed_states[0] corresponds to delay 1.0
        // dy/dt = -y(t) + 0.4*x(t - 0.5) + 0.3*y(t - 1.5)
        dydt[1] = -y[1] + 0.4 * y_delayed_states[1][0] + 0.3 * y_delayed_states[2][1];  // delay 0.5 and 1.5
        return dydt;
    };
    
    Func history_coupled = [](double t) 
    {
        return dVec {1.0, 0.7};
    };
    
    SolverParameters params_coupled;
    params_coupled.delay_derivative_multi = f_coupled;
    params_coupled.initial_conditions = {1.0, 0.7};
    params_coupled.t0 = 0.0;
    params_coupled.t1 = 8.0;
    params_coupled.dt = 0.1;
    params_coupled.delay_times = {1.0, 0.5, 1.5};  // Order matters for indexing in the function
    params_coupled.history_function = history_coupled;
    params_coupled.is_dde = true;
    
    SolverResults results_coupled = rk4_ralston_dde_general_solver(params_coupled);
    
    std::cout << "Solution at t = 8.0:" << std::endl;
    std::cout << "  x = " << std::fixed << std::setprecision(6) << results_coupled.solution.back()[0] << std::endl;
    std::cout << "  y = " << std::fixed << std::setprecision(6) << results_coupled.solution.back()[1] << std::endl;
    
    std::cout << "\nRK4 Ralston DDE solvers test completed successfully!" << std::endl;
    
    return 0;
} 