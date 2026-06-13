#include "../includes/solvers/DDE/rk/explicit/rk1-dde-solver.hpp"

int main() 
{
    // Example: Simple delay differential equation
    // dy/dt = -y(t) + 0.5 * y(t - 1.0)
    
    // Define the DDE function: f(t, y, y_delayed)
    DelayFunc f = [](double t, const dVec& y, const dVec& y_delayed) 
    {
        dVec  dydt(y.size());
        for (size_t i = 0; i < y.size(); ++i)
        {
            dydt[i] = -y[i] + 0.5 * y_delayed[i];
        }
        return dydt;
    };
    
    // Define the history function for t < t0
    Func history = [](double t) 
    {
        return dVec(1,1.0); // Constant history function
    };
    
    // Set up solver parameters
    SolverParameters params;
    params.delay_derivative = f;
    params.initial_conditions = {1.0};
    params.t0 = 0.0;
    params.t1 = 10.0;
    params.dt = 0.1;
    params.delay_times = {1.0};
    params.history_function = history;
    params.is_dde = true;
    
    std::cout << "Solving DDE: dy/dt = -y(t) + 0.5 * y(t - 1.0)" << std::endl;
    std::cout << "Initial condition: y(0) = 1.0" << std::endl;
    std::cout << "History function: y(t) = 1.0 for t < 0" << std::endl;
    std::cout << "Delay time: tau = 1.0" << std::endl;
    std::cout << std::endl;
    
    // Solve the DDE
    SolverResults results = rk1_dde_solver(params);
    
    // Print results
    std::cout << "Results (first 15 time points):" << std::endl;
    std::cout << std::setw(10) << "Time" << std::setw(15) << "y(t)" << std::endl;
    std::cout << std::string(25, '-') << std::endl;
    
    for (size_t i = 0; i < 15 && i < results.time_points.size(); ++i) 
    {
        std::cout << std::fixed << std::setprecision(3)
                  << std::setw(10) << results.time_points[i] 
                  << std::setw(15) << results.solution[i][0] << std::endl;
    }
    
    return 0;
} 