#include "../includes/solvers/DDE/rk/explicit/rk4-dde-solver.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk4-dde-general-solver.hpp"

int main() 
{
    std::cout << "=== RK4 DDE Solver Test ===" << std::endl;
    
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
    
    SolverResults results_single = rk4_dde_solver(params_single);
    
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
    
    SolverResults results_multi = rk4_dde_general_solver(params_multi);
    
    std::cout << "Solution at t = 5.0: " << std::fixed << std::setprecision(6) 
              << results_multi.solution.back()[0] << std::endl;
    
    std::cout << "\nRK4 DDE solvers test completed successfully!" << std::endl;
    
    return 0;
} 