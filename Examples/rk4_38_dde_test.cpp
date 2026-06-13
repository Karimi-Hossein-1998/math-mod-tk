#include "../includes/solvers/DDE/rk/explicit/rk4-dde-variants.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk4-dde-general-variants.hpp"

int main() 
{
    std::cout << "=== RK4 3/8 Rule DDE Solver Test ===" << std::endl;
    
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
    
    SolverResults results_single = rk4_38_dde_solver(params_single);
    
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
    params_multi.t1 = 50.0;
    params_multi.dt = 0.01;
    params_multi.delay_times = {0.5, 1.0};
    params_multi.history_function = history;
    params_multi.is_dde = true;
    
    SolverResults results_multi = rk4_38_dde_general_solver(params_multi);
    
    std::cout << "Solution at t = 5.0: " << std::fixed << std::setprecision(6) 
              << results_multi.solution.back()[0] << std::endl;
    
    // Test 3: Oscillatory DDE with single delay
    std::cout << "\nTest 3: Oscillatory DDE with Single Delay" << std::endl;
    std::cout << "Problem: dy/dt = -sin(y(t)) + 0.8*cos(y(t - 0.5))" << std::endl;
    
    DelayFunc f_osc = [](double t, const dVec& y, const dVec& y_delayed) 
    {
        dVec  dydt(y.size());
        for (size_t i = 0; i < y.size(); ++i)
        {
            dydt[i] = -std::sin(y[i]) + 0.8 * std::cos(y_delayed[i]);
        }
        return dydt;
    };
    
    Func history_osc = [](double t) 
    {
        return dVec {0.5};
    };
    
    SolverParameters params_osc;
    params_osc.delay_derivative = f_osc;
    params_osc.initial_conditions = {0.5};
    params_osc.t0 = 0.0;
    params_osc.t1 = 8.0;
    params_osc.dt = 0.05;
    params_osc.delay_times = {0.5};
    params_osc.history_function = history_osc;
    params_osc.is_dde = true;
    
    SolverResults results_osc = rk4_38_dde_solver(params_osc);
    
    std::cout << "Solution at t = 8.0: " << std::fixed << std::setprecision(6) 
              << results_osc.solution.back()[0] << std::endl;
    
    // Test 4: System of DDEs with multiple delays
    std::cout << "\nTest 4: System of DDEs with Multiple Delays" << std::endl;
    std::cout << "Problem: dx/dt = -x(t) + 0.5*y(t - 1.0)" << std::endl;
    std::cout << "         dy/dt = -y(t) + 0.3*x(t - 0.5) + 0.2*y(t - 1.5)" << std::endl;
    
    DelayFuncMulti f_system = [](double t, const dVec& y, const dMatrix& y_delayed_states) 
    {
        dVec  dydt(y.size());
        // dx/dt = -x(t) + 0.5*y(t - 1.0)
        dydt[0] = -y[0] + 0.5 * y_delayed_states[0][1];  // y_delayed_states[0] corresponds to delay 1.0
        // dy/dt = -y(t) + 0.3*x(t - 0.5) + 0.2*y(t - 1.5)
        dydt[1] = -y[1] + 0.3 * y_delayed_states[1][0] + 0.2 * y_delayed_states[2][1];  // delay 0.5 and 1.5
        return dydt;
    };
    
    Func history_system = [](double t) 
    {
        return dVec {1.0, 0.5};
    };
    
    SolverParameters params_system;
    params_system.delay_derivative_multi = f_system;
    params_system.initial_conditions = {1.0, 0.5};
    params_system.t0 = 0.0;
    params_system.t1 = 6.0;
    params_system.dt = 0.1;
    params_system.delay_times = {1.0, 0.5, 1.5};  // Order matters for indexing in the function
    params_system.history_function = history_system;
    params_system.is_dde = true;
    
    SolverResults results_system = rk4_38_dde_general_solver(params_system);
    
    std::cout << "Solution at t = 6.0:" << std::endl;
    std::cout << "  x = " << std::fixed << std::setprecision(6) << results_system.solution.back()[0] << std::endl;
    std::cout << "  y = " << std::fixed << std::setprecision(6) << results_system.solution.back()[1] << std::endl;
    
    std::cout << "\nRK4 3/8 Rule DDE solvers test completed successfully!" << std::endl;
    
    return 0;
} 