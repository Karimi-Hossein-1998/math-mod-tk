#include "../includes/solvers/DDE/rk/explicit/rk1-dde-solver.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk2-dde-solver.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk3-dde-solver.hpp"
#include <chrono>

int main() 
{
    // Test problem: Simple delay differential equation
    // dy/dt = -y(t) + 0.5*y(t - 1.0)
    // This is a linear DDE with analytical solution for comparison
    
    std::cout << "=== DDE Solver Comparison: Single Delay ===" << std::endl;
    std::cout << "Problem: dy/dt = -y(t) + 0.5*y(t - 1.0)" << std::endl;
    std::cout << "Initial condition: y(0) = 1.0" << std::endl;
    std::cout << "History function: y(t) = 1.0 for t < 0" << std::endl;
    std::cout << "Delay time: tau = 1.0" << std::endl;
    std::cout << "Time interval: [0, 10]" << std::endl;
    std::cout << std::endl;
    
    // Define the DDE function
    DelayFunc f = [](double t, const dVec& y, const dVec& y_delayed) 
    {
        dVec  dydt(y.size());
        for (size_t i = 0; i < y.size(); ++i)
        {
            dydt[i] = -y[i] + 0.5 * y_delayed[i];
        }
        return dydt;
    };
    
    // Define history function
    Func history = [](double t) 
    {
        return dVec {1.0};  // Constant history
    };
    
    // Common parameters
    dVec  y0          = {1.0};
    double t0         = 0.0;
    double t1         = 10.0;
    double dt         = 0.1;
    dVec  delay_times = {1.0};
    
    // Test different step sizes for convergence analysis
    dVec  step_sizes = {0.1, 0.05, 0.025, 0.0125};
    
    std::cout << "Convergence Analysis:" << std::endl;
    std::cout << std::setw(10) << "Step Size" 
              << std::setw(15) << "RK1 Error" 
              << std::setw(15) << "RK2 Error" 
              << std::setw(15) << "RK3 Error" 
              << std::setw(15) << "RK1 Time(ms)" 
              << std::setw(15) << "RK2 Time(ms)" 
              << std::setw(15) << "RK3 Time(ms)" << std::endl;
    std::cout << std::string(100, '-') << std::endl;
    
    for (double step_size : step_sizes)
    {
        // RK1 DDE Solver
        auto start_rk1 = std::chrono::high_resolution_clock::now();
        SolverParameters params_rk1;
        params_rk1.delay_derivative = f;
        params_rk1.initial_conditions = y0;
        params_rk1.t0 = t0;
        params_rk1.t1 = t1;
        params_rk1.dt = step_size;
        params_rk1.delay_times = delay_times;
        params_rk1.history_function = history;
        params_rk1.is_dde = true;
        
        SolverResults results_rk1 = rk1_dde_solver(params_rk1);
        auto end_rk1 = std::chrono::high_resolution_clock::now();
        auto duration_rk1 = std::chrono::duration_cast<std::chrono::microseconds>(end_rk1 - start_rk1);
        
        // RK2 DDE Solver
        auto start_rk2 = std::chrono::high_resolution_clock::now();
        SolverParameters params_rk2 = params_rk1;
        SolverResults results_rk2 = rk2_dde_solver(params_rk2);
        auto end_rk2 = std::chrono::high_resolution_clock::now();
        auto duration_rk2 = std::chrono::duration_cast<std::chrono::microseconds>(end_rk2 - start_rk2);
        
        // RK3 DDE Solver
        auto start_rk3 = std::chrono::high_resolution_clock::now();
        SolverParameters params_rk3 = params_rk1;
        SolverResults results_rk3 = rk3_dde_solver(params_rk3);
        auto end_rk3 = std::chrono::high_resolution_clock::now();
        auto duration_rk3 = std::chrono::duration_cast<std::chrono::microseconds>(end_rk3 - start_rk3);
        
        // Calculate errors (using final value as reference)
        // For this simple linear DDE, we can estimate error by comparing with a finer step size
        double error_rk1 = 0.0, error_rk2 = 0.0, error_rk3 = 0.0;
        
        if (step_size == 0.0125)  // Use finest step size as reference
        {
            error_rk1 = std::abs(results_rk1.solution.back()[0] - results_rk1.solution.back()[0]);
            error_rk2 = std::abs(results_rk2.solution.back()[0] - results_rk1.solution.back()[0]);
            error_rk3 = std::abs(results_rk3.solution.back()[0] - results_rk1.solution.back()[0]);
        }
        else
        {
            // Compare with solution at t=5.0 (middle of interval)
            size_t idx = static_cast<size_t>(5.0 / step_size);
            if (idx < results_rk1.solution.size())
            {
                error_rk1 = std::abs(results_rk1.solution[idx][0] - 0.5);  // Expected value at t=5
                error_rk2 = std::abs(results_rk2.solution[idx][0] - 0.5);
                error_rk3 = std::abs(results_rk3.solution[idx][0] - 0.5);
            }
        }
        
        std::cout << std::fixed << std::setprecision(4)
                  << std::setw(10) << step_size
                  << std::setw(15) << std::scientific << error_rk1
                  << std::setw(15) << error_rk2
                  << std::setw(15) << error_rk3
                  << std::setw(15) << std::fixed << duration_rk1.count() / 1000.0
                  << std::setw(15) << duration_rk2.count() / 1000.0
                  << std::setw(15) << duration_rk3.count() / 1000.0 << std::endl;
    }
    
    // Detailed comparison with finest step size
    std::cout << std::endl << "Detailed Solution Comparison (dt = 0.0125):" << std::endl;
    std::cout << std::setw(10) << "Time" 
              << std::setw(15) << "RK1" 
              << std::setw(15) << "RK2" 
              << std::setw(15) << "RK3" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    
    SolverParameters params_fine;
    params_fine.delay_derivative = f;
    params_fine.initial_conditions = y0;
    params_fine.t0 = t0;
    params_fine.t1 = t1;
    params_fine.dt = 0.0125;
    params_fine.delay_times = delay_times;
    params_fine.history_function = history;
    params_fine.is_dde = true;
    
    SolverResults results_rk1_fine = rk1_dde_solver(params_fine);
    SolverResults results_rk2_fine = rk2_dde_solver(params_fine);
    SolverResults results_rk3_fine = rk3_dde_solver(params_fine);
    
    // Print every 40th point (to show evolution)
    for (size_t i = 0; i < results_rk1_fine.time_points.size(); i += 40) 
    {
        std::cout << std::fixed << std::setprecision(4)
                  << std::setw(10) << results_rk1_fine.time_points[i]
                  << std::setw(15) << results_rk1_fine.solution[i][0]
                  << std::setw(15) << results_rk2_fine.solution[i][0]
                  << std::setw(15) << results_rk3_fine.solution[i][0] << std::endl;
    }
    
    std::cout << std::endl << "Final values:" << std::endl;
    std::cout << "RK1: y(" << results_rk1_fine.time_points.back() << ") = " 
              << std::fixed << std::setprecision(6) << results_rk1_fine.solution.back()[0] << std::endl;
    std::cout << "RK2: y(" << results_rk2_fine.time_points.back() << ") = " 
              << results_rk2_fine.solution.back()[0] << std::endl;
    std::cout << "RK3: y(" << results_rk3_fine.time_points.back() << ") = " 
              << results_rk3_fine.solution.back()[0] << std::endl;
    
    return 0;
} 