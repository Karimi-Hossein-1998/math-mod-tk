#include "../includes/solvers/DDE/rk/explicit/rk2-dde-solver.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk1-dde-solver.hpp"

int main() 
{
    // Example: Comparing RK1 and RK2 DDE solvers
    // dy/dt = -y(t) + 0.5 * y(t - 1.0)
    // This example shows the improved accuracy of RK2 over RK1
    
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
        return dVec(1, 1.0); // Constant history function
    };
    
    // Set up solver parameters
    SolverParameters params;
    params.delay_derivative = f;
    params.initial_conditions = {1.0};
    params.t0 = 0.0;
    params.t1 = 5.0;
    params.dt = 0.1;
    params.delay_times = {1.0};
    params.history_function = history;
    params.is_dde = true;
    
    std::cout << "Comparing RK1 and RK2 DDE Solvers" << std::endl;
    std::cout << "DDE: dy/dt = -y(t) + 0.5 * y(t - 1.0)" << std::endl;
    std::cout << "Initial condition: y(0) = 1.0" << std::endl;
    std::cout << "History function: y(t) = 1.0 for t < 0" << std::endl;
    std::cout << "Delay time: tau = 1.0" << std::endl;
    std::cout << "Time interval: [0, 5]" << std::endl;
    std::cout << "Step size: dt = 0.1" << std::endl;
    std::cout << std::endl;
    
    // Solve with RK2 DDE solver
    SolverResults results_rk2 = rk2_dde_solver(params);
    
    // For comparison, also solve with RK1 (using the same parameters)
    SolverResults results_rk1 = rk1_dde_solver(params);
    
    // Print comparison results
    std::cout << "Comparison of RK1 vs RK2 DDE Solvers:" << std::endl;
    std::cout << std::setw(10) << "Time" 
              << std::setw(15) << "RK1 y(t)" 
              << std::setw(15) << "RK2 y(t)" 
              << std::setw(15) << "Difference" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    
    for (size_t i = 0; i < 20 && i < results_rk1.time_points.size(); ++i) 
    {
        double diff = std::abs(results_rk2.solution[i][0] - results_rk1.solution[i][0]);
        std::cout << std::fixed << std::setprecision(3)
                  << std::setw(10) << results_rk1.time_points[i] 
                  << std::setw(15) << results_rk1.solution[i][0]
                  << std::setw(15) << results_rk2.solution[i][0]
                  << std::setw(15) << diff << std::endl;
    }
    
    // Print final values and overall accuracy comparison
    if (!results_rk1.solution.empty() && !results_rk2.solution.empty()) 
    {
        std::cout << std::endl << "Final values:" << std::endl;
        std::cout << "RK1: y(" << results_rk1.time_points.back() << ") = " 
                  << std::fixed << std::setprecision(6) << results_rk1.solution.back()[0] << std::endl;
        std::cout << "RK2: y(" << results_rk2.time_points.back() << ") = " 
                  << std::fixed << std::setprecision(6) << results_rk2.solution.back()[0] << std::endl;
        
        // Calculate maximum difference
        double max_diff = 0.0;
        for (size_t i = 0; i < results_rk1.solution.size(); ++i) 
        {
            double diff = std::abs(results_rk2.solution[i][0] - results_rk1.solution[i][0]);
            if (diff > max_diff) max_diff = diff;
        }
        std::cout << "Maximum difference between RK1 and RK2: " 
                  << std::fixed << std::setprecision(6) << max_diff << std::endl;
    }
    
    return 0;
} 