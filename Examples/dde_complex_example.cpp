#include "../includes/solvers/DDE/rk/explicit/rk1-dde-solver.hpp"

int main() 
{
    // Example: More complex delay differential equation
    // dy/dt = -2*y(t) + sin(y(t - 0.5)) + 0.3*y(t - 1.0)
    // This is a nonlinear DDE with two delays
    
    // Define the DDE function: f(t, y, y_delayed)
    DelayFunc f = [](double t, const dVec& y, const dVec& y_delayed) 
    {
        dVec  dydt(y.size());
        for (size_t i = 0; i < y.size(); ++i)
        {
            // For simplicity, we'll use the first delay (0.5) in this example
            // In a full implementation, you might want to handle multiple delays
            dydt[i] = -2.0 * y[i] + std::sin(y_delayed[i]) + 0.3 * y_delayed[i];
        }
        return dydt;
    };
    
    // Define a more interesting history function
    Func history = [](double t) 
    {
        // Use a sinusoidal history function
        return dVec {std::sin(t)};
    };
    
    // Set up solver parameters
    SolverParameters params;
    params.delay_derivative = f;
    params.initial_conditions = {0.5};  // y(0) = 0.5
    params.t0 = 0.0;
    params.t1 = 15.0;
    params.dt = 0.05;  // Smaller step size for better accuracy
    params.delay_times = {0.5};  // Primary delay of 0.5
    params.history_function = history;
    params.is_dde = true;
    
    std::cout << "Solving Complex DDE: dy/dt = -2*y(t) + sin(y(t - 0.5)) + 0.3*y(t - 0.5)" << std::endl;
    std::cout << "Initial condition: y(0) = 0.5" << std::endl;
    std::cout << "History function: y(t) = sin(t) for t < 0" << std::endl;
    std::cout << "Delay time: tau = 0.5" << std::endl;
    std::cout << "Time interval: [0, 15]" << std::endl;
    std::cout << "Step size: dt = 0.05" << std::endl;
    std::cout << std::endl;
    
    // Solve the DDE
    SolverResults results = rk1_dde_solver(params);
    
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
    
    // Print some intermediate points
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