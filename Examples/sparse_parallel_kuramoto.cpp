#include "../includes/solvers/solver.hpp"
#include "../includes/models/kuramoto.hpp"
#include "../includes/network/topology.hpp"
#include "../includes/initializers/initials.hpp"

int main() 
{
    size_t N = 1000;
    double K = 0.5;
    double alpha = 0.1;
    unsigned seed = 123;
    // Generate random initial phases and frequencies
    dVec  theta0 = random_uniform(N, -PI, PI, seed);
    dVec  omega = random_normal(N, 0.0, 0.5, seed+1);
    // Generate a sparse small-world adjacency matrix
    SparsedMatrix sparse_adj = small_world_sparse(N, 10, 0.2, 1.0, seed+2);
    // Define the parallel Kuramoto ODE
    MyFunc kuramoto = [&](double t, const dVec& theta) 
    {
        return kuramoto_sparse_parallel(t, theta, omega, K, sparse_adj, alpha);
    };
    SolverParameters params;
    params.derivative = kuramoto;
    params.initial_conditions = theta0;
    params.t0 = 0.0;
    params.t1 = 2.0;
    params.dt = 0.01;
    SolverResults results = rk4_solver(params);
    // Print first 5 time points
    for (size_t i = 0; i < 5 && i < results.time_points.size(); ++i) 
    {
        std::cout << "t = " << results.time_points[i] << ", theta[0] = " << results.solution[i][0] << std::endl;
    }
    return 0;
} 