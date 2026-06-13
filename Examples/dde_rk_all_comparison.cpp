#include "../includes/solvers/DDE/rk/explicit/rk1-dde-solver.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk2-dde-solver.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk3-dde-solver.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk4-dde-solver.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk1-dde-general-solver.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk2-dde-general-solver.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk3-dde-general-solver.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk4-dde-general-solver.hpp"
#include <chrono>

int main() 
{
    std::cout << "=== Complete DDE Solver Comparison: RK1, RK2, RK3, RK4 ===" << std::endl;
    
    // Test problem: dy/dt = -y(t) + 0.5*y(t - 1.0)
    std::cout << "Problem: dy/dt = -y(t) + 0.5*y(t - 1.0)" << std::endl;
    std::cout << "Initial condition: y(0) = 1.0" << std::endl;
    std::cout << "History function: y(t) = 1.0 for t < 0" << std::endl;
    std::cout << "Delay time: tau = 1.0" << std::endl;
    std::cout << "Time interval: [0, 5]" << std::endl;
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
        return dVec {1.0};
    };
    
    // Common parameters
    dVec  y0 = {1.0};
    double t0 = 0.0;
    double t1 = 500.0;
    dVec  delay_times = {1.0};
    
    // Test different step sizes
    std::vector<double> step_sizes = {0.1, 0.05, 0.025, 0.0125};
    
    std::cout << "Convergence Analysis (solution at t = 5.0):" << std::endl;
    std::cout << std::setw(10) << "Step Size" 
              << std::setw(15) << "RK1" 
              << std::setw(15) << "RK2" 
              << std::setw(15) << "RK3" 
              << std::setw(15) << "RK4" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    std::vector<std::vector<double>> all_results(4); // Store results for each method
    
    for (double dt : step_sizes)
    {
        // RK1
        auto start_rk1 = std::chrono::high_resolution_clock::now();
        SolverParameters params_rk1;
        params_rk1.delay_derivative = f;
        params_rk1.initial_conditions = y0;
        params_rk1.t0 = t0;
        params_rk1.t1 = t1;
        params_rk1.dt = dt;
        params_rk1.delay_times = delay_times;
        params_rk1.history_function = history;
        params_rk1.is_dde = true;
        
        SolverResults results_rk1 = rk1_dde_solver(params_rk1);
        auto end_rk1 = std::chrono::high_resolution_clock::now();
        auto duration_rk1 = std::chrono::duration_cast<std::chrono::microseconds>(end_rk1 - start_rk1);
        
        // RK2
        auto start_rk2 = std::chrono::high_resolution_clock::now();
        SolverParameters params_rk2 = params_rk1;
        SolverResults results_rk2 = rk2_dde_solver(params_rk2);
        auto end_rk2 = std::chrono::high_resolution_clock::now();
        auto duration_rk2 = std::chrono::duration_cast<std::chrono::microseconds>(end_rk2 - start_rk2);
        
        // RK3
        auto start_rk3 = std::chrono::high_resolution_clock::now();
        SolverParameters params_rk3 = params_rk1;
        SolverResults results_rk3 = rk3_dde_solver(params_rk3);
        auto end_rk3 = std::chrono::high_resolution_clock::now();
        auto duration_rk3 = std::chrono::duration_cast<std::chrono::microseconds>(end_rk3 - start_rk3);
        
        // RK4
        auto start_rk4 = std::chrono::high_resolution_clock::now();
        SolverParameters params_rk4 = params_rk1;
        SolverResults results_rk4 = rk4_dde_solver(params_rk4);
        auto end_rk4 = std::chrono::high_resolution_clock::now();
        auto duration_rk4 = std::chrono::duration_cast<std::chrono::microseconds>(end_rk4 - start_rk4);
        
        // Store results
        all_results[0].push_back(results_rk1.solution.back()[0]);
        all_results[1].push_back(results_rk2.solution.back()[0]);
        all_results[2].push_back(results_rk3.solution.back()[0]);
        all_results[3].push_back(results_rk4.solution.back()[0]);
        
        std::cout << std::fixed << std::setprecision(4)
                  << std::setw(10) << dt
                  << std::setw(15) << std::setprecision(6) << results_rk1.solution.back()[0]
                  << std::setw(15) << results_rk2.solution.back()[0]
                  << std::setw(15) << results_rk3.solution.back()[0]
                  << std::setw(15) << results_rk4.solution.back()[0] << std::endl;
    }
    
    // Calculate convergence rates
    std::cout << std::endl << "Convergence Rate Analysis:" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    for (size_t method = 0; method < 4; ++method)
    {
        std::string method_name = "RK" + std::to_string(method + 1);
        
        // Calculate convergence rate using last two points
        if (all_results[method].size() >= 2)
        {
            size_t n = all_results[method].size();
            double change = std::abs(all_results[method][n-2] - all_results[method][n-1]);
            double step_ratio = step_sizes[n-2] / step_sizes[n-1];
            double rate = std::log(change) / std::log(step_ratio);
            
            std::cout << method_name << " | Change: " << std::scientific << std::setprecision(2) 
                      << std::setw(10) << change << " | Rate: " << std::fixed << std::setprecision(2) 
                      << std::setw(6) << rate << std::endl;
        }
    }
    
    // Performance comparison
    std::cout << std::endl << "Performance Comparison (dt = 0.0125):" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    
    // Run timing test with finest step size
    SolverParameters params_fine;
    params_fine.delay_derivative = f;
    params_fine.initial_conditions = y0;
    params_fine.t0 = t0;
    params_fine.t1 = t1;
    params_fine.dt = 0.0125;
    params_fine.delay_times = delay_times;
    params_fine.history_function = history;
    params_fine.is_dde = true;
    
    auto start_rk1 = std::chrono::high_resolution_clock::now();
    SolverResults results_rk1 = rk1_dde_solver(params_fine);
    auto end_rk1 = std::chrono::high_resolution_clock::now();
    auto duration_rk1 = std::chrono::duration_cast<std::chrono::microseconds>(end_rk1 - start_rk1);
    
    auto start_rk2 = std::chrono::high_resolution_clock::now();
    SolverResults results_rk2 = rk2_dde_solver(params_fine);
    auto end_rk2 = std::chrono::high_resolution_clock::now();
    auto duration_rk2 = std::chrono::duration_cast<std::chrono::microseconds>(end_rk2 - start_rk2);
    
    auto start_rk3 = std::chrono::high_resolution_clock::now();
    SolverResults results_rk3 = rk3_dde_solver(params_fine);
    auto end_rk3 = std::chrono::high_resolution_clock::now();
    auto duration_rk3 = std::chrono::duration_cast<std::chrono::microseconds>(end_rk3 - start_rk3);
    
    auto start_rk4 = std::chrono::high_resolution_clock::now();
    SolverResults results_rk4 = rk4_dde_solver(params_fine);
    auto end_rk4 = std::chrono::high_resolution_clock::now();
    auto duration_rk4 = std::chrono::duration_cast<std::chrono::microseconds>(end_rk4 - start_rk4);
    
    std::cout << "Method | Time (ms) | Relative Cost" << std::endl;
    std::cout << "-------|-----------|--------------" << std::endl;
    std::cout << "RK1    | " << std::fixed << std::setprecision(2) << std::setw(9) << duration_rk1.count() / 1000.0 
              << " | 1.0x" << std::endl;
    std::cout << "RK2    | " << std::fixed << std::setprecision(2) << std::setw(9) << duration_rk2.count() / 1000.0 
              << " | " << std::fixed << std::setprecision(1) << duration_rk2.count() / (double)duration_rk1.count() << "x" << std::endl;
    std::cout << "RK3    | " << std::fixed << std::setprecision(2) << std::setw(9) << duration_rk3.count() / 1000.0 
              << " | " << std::fixed << std::setprecision(1) << duration_rk3.count() / (double)duration_rk1.count() << "x" << std::endl;
    std::cout << "RK4    | " << std::fixed << std::setprecision(2) << std::setw(9) << duration_rk4.count() / 1000.0 
              << " | " << std::fixed << std::setprecision(1) << duration_rk4.count() / (double)duration_rk1.count() << "x" << std::endl;
    
    // Final comparison
    std::cout << std::endl << "Final Solution Comparison (dt = 0.0125):" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "RK1: y(5.0) = " << std::fixed << std::setprecision(6) << results_rk1.solution.back()[0] << std::endl;
    std::cout << "RK2: y(5.0) = " << std::fixed << std::setprecision(6) << results_rk2.solution.back()[0] << std::endl;
    std::cout << "RK3: y(5.0) = " << std::fixed << std::setprecision(6) << results_rk3.solution.back()[0] << std::endl;
    std::cout << "RK4: y(5.0) = " << std::fixed << std::setprecision(6) << results_rk4.solution.back()[0] << std::endl;
    
    // Calculate differences from RK4 (highest order)
    double rk4_value = results_rk4.solution.back()[0];
    std::cout << std::endl << "Differences from RK4:" << std::endl;
    std::cout << std::string(30, '-') << std::endl;
    std::cout << "RK1: " << std::scientific << std::setprecision(2) << std::abs(results_rk1.solution.back()[0] - rk4_value) << std::endl;
    std::cout << "RK2: " << std::scientific << std::setprecision(2) << std::abs(results_rk2.solution.back()[0] - rk4_value) << std::endl;
    std::cout << "RK3: " << std::scientific << std::setprecision(2) << std::abs(results_rk3.solution.back()[0] - rk4_value) << std::endl;
    std::cout << "RK4: " << std::scientific << std::setprecision(2) << 0.0 << std::endl;
    
    std::cout << std::endl << "Summary:" << std::endl;
    std::cout << "- RK4 provides the highest accuracy (4th order)" << std::endl;
    std::cout << "- RK3 and RK4 should be very close (both high order)" << std::endl;
    std::cout << "- RK1 shows the largest differences (1st order)" << std::endl;
    std::cout << "- Higher order methods are more computationally expensive" << std::endl;
    
    return 0;
} 