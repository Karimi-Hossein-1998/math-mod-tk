#include "../includes/solvers/DDE/rk/explicit/rk1-dde-solver.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk2-dde-solver.hpp"
#include "../includes/solvers/DDE/rk/explicit/rk3-dde-solver.hpp"
#include <cmath>

// Function to calculate convergence rate
double calculate_convergence_rate(const std::vector<double>& errors, const std::vector<double>& step_sizes) 
{
    if (errors.size() < 2 || step_sizes.size() < 2) return 0.0;
    
    // Calculate convergence rate using the last two points
    // rate = log(error1/error2) / log(step1/step2)
    size_t n = errors.size();
    double error_ratio = errors[n-2] / errors[n-1];
    double step_ratio = step_sizes[n-2] / step_sizes[n-1];
    
    return std::log(error_ratio) / std::log(step_ratio);
}

int main() 
{
    std::cout << "=== Proper DDE Error Analysis Example ===" << std::endl;
    std::cout << "Problem: dy/dt = -y(t) + 0.5*y(t - 1.0)" << std::endl;
    std::cout << "Initial condition: y(0) = 1.0" << std::endl;
    std::cout << "History function: y(t) = 1.0 for t < 0" << std::endl;
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
    double t1 = 5.0;  // Shorter interval for faster computation
    dVec  delay_times = {1.0};
    
    // Test different step sizes for convergence analysis
    std::vector<double> step_sizes = {0.1, 0.05, 0.025, 0.0125, 0.00625};
    
    // Store errors for each method
    std::vector<double> errors_rk1, errors_rk2, errors_rk3;
    
    std::cout << "Step Size Analysis:" << std::endl;
    std::cout << std::setw(10) << "Step Size" 
              << std::setw(15) << "RK1 Error" 
              << std::setw(15) << "RK2 Error" 
              << std::setw(15) << "RK3 Error" << std::endl;
    std::cout << std::string(55, '-') << std::endl;
    
    // First, compute a very fine reference solution using RK3 with smallest step size
    SolverParameters params_reference;
    params_reference.delay_derivative = f;
    params_reference.initial_conditions = y0;
    params_reference.t0 = t0;
    params_reference.t1 = t1;
    params_reference.dt = 0.001;  // Very fine step size for reference
    params_reference.delay_times = delay_times;
    params_reference.history_function = history;
    params_reference.is_dde = true;
    
    SolverResults reference_solution = rk3_dde_solver(params_reference);
    
    // Now test each step size
    for (double step_size : step_sizes)
    {
        // RK1 DDE Solver
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
        
        // RK2 DDE Solver
        SolverParameters params_rk2 = params_rk1;
        SolverResults results_rk2 = rk2_dde_solver(params_rk2);
        
        // RK3 DDE Solver
        SolverParameters params_rk3 = params_rk1;
        SolverResults results_rk3 = rk3_dde_solver(params_rk3);
        
        // Calculate errors by comparing with reference solution at t = 3.0
        double t_compare = 3.0;
        
        // Find index in reference solution
        size_t ref_idx = static_cast<size_t>((t_compare - t0) / 0.001);
        if (ref_idx >= reference_solution.solution.size()) ref_idx = reference_solution.solution.size() - 1;
        double ref_value = reference_solution.solution[ref_idx][0];
        
        // Find index in each solution
        size_t idx_rk1 = static_cast<size_t>((t_compare - t0) / step_size);
        size_t idx_rk2 = idx_rk1;
        size_t idx_rk3 = idx_rk1;
        
        if (idx_rk1 >= results_rk1.solution.size()) idx_rk1 = results_rk1.solution.size() - 1;
        if (idx_rk2 >= results_rk2.solution.size()) idx_rk2 = results_rk2.solution.size() - 1;
        if (idx_rk3 >= results_rk3.solution.size()) idx_rk3 = results_rk3.solution.size() - 1;
        
        double error_rk1 = std::abs(results_rk1.solution[idx_rk1][0] - ref_value);
        double error_rk2 = std::abs(results_rk2.solution[idx_rk2][0] - ref_value);
        double error_rk3 = std::abs(results_rk3.solution[idx_rk3][0] - ref_value);
        
        errors_rk1.push_back(error_rk1);
        errors_rk2.push_back(error_rk2);
        errors_rk3.push_back(error_rk3);
        
        std::cout << std::fixed << std::setprecision(4)
                  << std::setw(10) << step_size
                  << std::setw(15) << std::scientific << error_rk1
                  << std::setw(15) << error_rk2
                  << std::setw(15) << error_rk3 << std::endl;
    }
    
    // Calculate convergence rates
    double rate_rk1 = calculate_convergence_rate(errors_rk1, step_sizes);
    double rate_rk2 = calculate_convergence_rate(errors_rk2, step_sizes);
    double rate_rk3 = calculate_convergence_rate(errors_rk3, step_sizes);
    
    std::cout << std::endl << "Convergence Analysis:" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    std::cout << "Method | Expected Order | Measured Order | Error at dt=0.1" << std::endl;
    std::cout << "-------|----------------|----------------|----------------" << std::endl;
    std::cout << "RK1    | 1st order      | " << std::fixed << std::setprecision(2) << std::setw(14) << rate_rk1 
              << " | " << std::scientific << std::setprecision(2) << std::setw(14) << errors_rk1[0] << std::endl;
    std::cout << "RK2    | 2nd order      | " << std::fixed << std::setprecision(2) << std::setw(14) << rate_rk2 
              << " | " << std::scientific << std::setprecision(2) << std::setw(14) << errors_rk2[0] << std::endl;
    std::cout << "RK3    | 3rd order      | " << std::fixed << std::setprecision(2) << std::setw(14) << rate_rk3 
              << " | " << std::scientific << std::setprecision(2) << std::setw(14) << errors_rk3[0] << std::endl;
    
    // Show error reduction factors
    std::cout << std::endl << "Error Reduction Analysis (comparing to RK1):" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    std::cout << "Method | Error Reduction Factor" << std::endl;
    std::cout << "-------|----------------------" << std::endl;
    std::cout << "RK1    | 1.0x (baseline)" << std::endl;
    std::cout << "RK2    | " << std::fixed << std::setprecision(1) << errors_rk1[0] / errors_rk2[0] << "x more accurate" << std::endl;
    std::cout << "RK3    | " << std::fixed << std::setprecision(1) << errors_rk1[0] / errors_rk3[0] << "x more accurate" << std::endl;
    
    // Show how errors scale with step size
    std::cout << std::endl << "Error Scaling Analysis:" << std::endl;
    std::cout << "Step Size | RK1 Error | RK2 Error | RK3 Error" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
    
    for (size_t i = 0; i < step_sizes.size(); ++i) 
    {
        std::cout << std::fixed << std::setprecision(4) << std::setw(10) << step_sizes[i]
                  << std::setw(11) << std::scientific << std::setprecision(2) << errors_rk1[i]
                  << std::setw(11) << errors_rk2[i]
                  << std::setw(11) << errors_rk3[i] << std::endl;
    }
    
    std::cout << std::endl << "Interpretation:" << std::endl;
    std::cout << "- Convergence rate should be close to the theoretical order" << std::endl;
    std::cout << "- RK2 should be ~2x more accurate than RK1" << std::endl;
    std::cout << "- RK3 should be ~3x more accurate than RK1" << std::endl;
    std::cout << "- Errors should decrease as step size decreases" << std::endl;
    
    return 0;
} 