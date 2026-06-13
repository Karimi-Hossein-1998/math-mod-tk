#include "../includes/solvers/DDE/rk/explicit/rk1-dde-general-solver.hpp"

int main() 
{
    // Example: Multiple delay differential equation
    // dy/dt = -2*y(t) + 0.5*y(t - 0.5) + 0.3*y(t - 1.0)
    // This is a DDE with two different delays: τ₁ = 0.5 and τ₂ = 1.0
    
    // Define the multiple-delay DDE function: f(t, y, y_delayed_states)
    // y_delayed_states[0] = y(t - 0.5), y_delayed_states[1] = y(t - 1.0)
    DelayFuncMulti f = [](double t, const dVec& y, const dMatrix& y_delayed_states) 
    {
        dVec  dydt(y.size());
        for (size_t i = 0; i < y.size(); ++i)
        {
            // dy/dt = -2*y(t) + 0.5*y(t - 0.5) + 0.3*y(t - 1.0)
            dydt[i] = -2.0 * y[i] + 0.5 * y_delayed_states[0][i] + 0.3 * y_delayed_states[1][i];
        }
        return dydt;
    };
    
    // Define the history function for t < t0
    Func history = [](double t) 
    {
        return dVec {std::sin(t)}; // Sinusoidal history function
    };
    
    // Set up solver parameters
    SolverParameters params;
    params.delay_derivative_multi = f;
    params.initial_conditions = {0.5};
    params.t0 = 0.0;
    params.t1 = 10.0;
    params.dt = 0.05;
    params.delay_times = {0.5, 1.0};  // Two delays: τ₁ = 0.5, τ₂ = 1.0
    params.history_function = history;
    params.is_dde = true;
    
    std::cout << "Solving Multiple-Delay DDE:" << std::endl;
    std::cout << "dy/dt = -2*y(t) + 0.5*y(t - 0.5) + 0.3*y(t - 1.0)" << std::endl;
    std::cout << "Initial condition: y(0) = 0.5" << std::endl;
    std::cout << "History function: y(t) = sin(t) for t < 0" << std::endl;
    std::cout << "Delay times: τ₁ = 0.5, τ₂ = 1.0" << std::endl;
    std::cout << "Time interval: [0, 10]" << std::endl;
    std::cout << "Step size: dt = 0.05" << std::endl;
    std::cout << std::endl;
    
    // Solve the multiple-delay DDE
    SolverResults results = rk1_dde_general_solver(params);
    
    // Print results
    std::cout << "Results (first 20 time points):" << std::endl;
    std::cout << std::setw(10) << "Time" << std::setw(15) << "y(t)" << std::endl;
    std::cout << std::string(25, '-') << std::endl;
    
    for (size_t i = 0; i < 20 && i < results.time_points.size(); ++i) 
    {
        std::cout << std::fixed << std::setprecision(4)
                  << std::setw(10) << results.time_points[i] 
                  << std::setw(15) << results.solution[i][0] << std::endl;
    }
    
    // Print some intermediate points to show the solution behavior
    std::cout << std::endl << "Intermediate points:" << std::endl;
    std::cout << std::setw(10) << "Time" << std::setw(15) << "y(t)" << std::endl;
    std::cout << std::string(25, '-') << std::endl;
    
    for (size_t i = 50; i < results.time_points.size(); i += 50) 
    {
        std::cout << std::fixed << std::setprecision(4)
                  << std::setw(10) << results.time_points[i] 
                  << std::setw(15) << results.solution[i][0] << std::endl;
    }
    
    // Print final value
    if (!results.solution.empty()) 
    {
        std::cout << std::endl << "Final value:" << std::endl;
        std::cout << "y(" << results.time_points.back() << ") = " 
                  << std::fixed << std::setprecision(6) << results.solution.back()[0] << std::endl;
    }
    
    return 0;
} 