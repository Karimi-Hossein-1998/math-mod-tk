#include "../includes/solvers/solver.hpp"
#include "../includes/models/kuramoto.hpp"

int main() 
{
    // System parameters
    const size_t N = 100;                    // Number of oscillators
    const double K = 2.0;                    // Coupling strength
    const double alpha = 0.0;                // Phase lag parameter
    
    // Create initial conditions and natural frequencies
    dVec  theta0(N);
    dVec  omega(N);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> theta_dist(-PI, PI);
    std::normal_distribution<> omega_dist(0.0, 1.0);
    
    for (size_t i = 0; i < N; ++i) 
    {
        theta0[i] = theta_dist(gen);
        omega[i] = omega_dist(gen);
    }
    
    // Create adjacency matrix (all-to-all coupling)
    dMatrix adj(N, dVec(N, 1.0));
    for (size_t i = 0; i < N; ++i) 
    {
        adj[i][i] = 0.0;  // No self-coupling
    }
    
    // Create solver parameters
    SolverParameters params;
    params.initial_conditions = theta0;
    params.t0 = 0.0;
    params.t1 = 50.0;
    params.dt = 0.01;
    params.derivative = [&](double t, const dVec& theta) 
    {
        return kuramoto_general(t, theta, omega, K, adj, alpha);
    };
    
    // Solve the system
    std::cout << "Solving Kuramoto model..." << std::endl;
    auto results = rk4_solver(params);
    
    // Calculate order parameter evolution
    std::cout << "Calculating order parameter..." << std::endl;
    auto order = calculate_order(results);
    
    // Print results
    std::cout << "\nTime evolution of order parameter:" << std::endl;
    std::cout << std::setw(15) << "Time" 
              << std::setw(15) << "Magnitude" 
              << std::setw(15) << "Sin comp." 
              << std::setw(15) << "Cos comp." << std::endl;
    std::cout << std::string(60, '-') << std::endl;
    
    // Print every 100th point to keep output manageable
    for (size_t i = 0; i < order.size(); i += 100)
    {
        std::cout << std::fixed << std::setprecision(6)
                  << std::setw(15) << order[i][3]  // Time
                  << std::setw(15) << order[i][2]  // Magnitude
                  << std::setw(15) << order[i][0]  // Sin component
                  << std::setw(15) << order[i][1]  // Cos component
                  << std::endl;
    }
    
    return 0;
} 