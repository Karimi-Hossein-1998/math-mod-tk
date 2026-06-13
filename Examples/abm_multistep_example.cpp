#include "../includes/solvers/solver.hpp"

// dy/dt = sin(t), y(0) = 0
int main() {
    MyFunc f = [](double t, const dVec& y) 
    {
        dVec  dydt(y.size());
        for (size_t i = 0; i < y.size(); ++i)
            dydt[i] = std::sin(t);
        return dydt;
    };
    SolverParameters params;
    params.derivative = f;
    params.initial_conditions = {0.0};
    params.t0 = 0.0;
    params.t1 = 6.28; // ~2*pi
    params.dt = 0.1;
    params.order = 5; // Use 5th order ABM
    params.iterations = 2; // Number of corrector iterations
    SolverResults results = adams_bashforth_moulton_solver(params);
    // Print first 10 time points
    for (size_t i = 0; i < 10 && i < results.time_points.size(); ++i) 
    {
        std::cout << "t = " << results.time_points[i] << ", y = " << results.solution[i][0] << std::endl;
    }
    return 0;
} 