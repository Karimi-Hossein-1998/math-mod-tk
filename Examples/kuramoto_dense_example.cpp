#include "../includes/solvers/solver.hpp"
#include "../includes/models/kuramoto.hpp"
#include "../includes/initializers/initials.hpp"
#include "../includes/network/topology.hpp"

int main() 
{
    size_t N = 10;
    double K = 1.0;
    double alpha = 0.0;
    unsigned seed = 42;
    // Generate random initial phases and frequencies
    dVec  theta0 = random_uniform(N, -PI, PI, seed);
    dVec  omega  = random_normal(N, 0.0, 1.0, seed+1);
    // Generate a random dense adjacency matrix
    dMatrix adj = random(N, 0.0, 1.0, seed+2);
    // Define the Kuramoto ODE
    MyFunc kuramoto = [&](double t, const dVec& theta) 
    {
        return kuramoto_general(t, theta, omega, K, adj, alpha);
    };
    SolverParameters params;
    params.derivative = kuramoto;
    params.initial_conditions = theta0;
    params.t0 = 0.0;
    params.t1 = 10.0;
    params.dt = 0.05;
    SolverResults results = rk4_solver(params);
    // Print first 5 time points
    for (size_t i = 0; i < 5 && i < results.time_points.size(); ++i) 
    {
        std::cout << "t = " << results.time_points[i] << ", theta[0] = " << results.solution[i][0] << std::endl;
    }
    return 0;
} 