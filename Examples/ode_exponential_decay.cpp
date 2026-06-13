#include "../includes/solvers/solver.hpp"

// dy/dt = -y, y(0) = 1
int main() 
{
    MyFunc f = [](double t, const dVec& y) 
    {
        dVec  dydt(y.size());
        for (size_t i = 0; i < y.size(); ++i)
            dydt[i] = -y[i];
        return dydt;
    };
    SolverParameters params;
    params.derivative = f;
    params.initial_conditions = {1.0};
    params.t0 = 0.0;
    params.t1 = 5.0;
    params.dt = 0.1;
    SolverResults results = rk4_solver(params);
    // Print results
    for (size_t i = 0; i < results.time_points.size(); ++i) 
    {
        std::cout << "t = " << results.time_points[i] << ", y = " << results.solution[i][0] << std::endl;
    }
    return 0;
} 