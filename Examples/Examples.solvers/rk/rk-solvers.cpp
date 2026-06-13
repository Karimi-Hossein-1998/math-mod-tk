#include "../../../includes/solvers/ODE/rk/explicit/rk1-solver.hpp"
#include "../../../includes/solvers/ODE/rk/explicit/rk2-solver.hpp"
#include "../../../includes/solvers/ODE/rk/explicit/rk3-solver.hpp"
#include "../../../includes/solvers/ODE/rk/explicit/rk4-solver.hpp"
#include "../../../includes/solvers/ODE/rk/explicit/rk4-variants.hpp"
#include "../../../includes/utility/write.hpp"

inline dVec unwrapped_decay(double lambda, double t, const dVec &y)
{
    size_t n = y.size();
    dVec dydt = dVec(n, 0.0);
    for (size_t i = 0; i < n; ++i)
    {
        dydt[i] = - lambda * y[i];
    }
    return dydt;
}

inline MyFunc decay (double lambda)
{
    return [lambda](double t, const dVec &y) -> dVec
    {
        return unwrapped_decay(lambda, t, y);
    };
}

inline dVec exact_solution(double lambda, double t, const dVec y0)
{
    dVec res = dVec(y0.size(),0.0);
    for (size_t i = 0; i < y0.size(); ++i)
    {
        res[i] = y0[i] * exp( - lambda*t);
    }
    return res;
}

int main()
{
    double lambda = 0.8;
    SolverParameters params;
    params.dt = 1.0 / 16.0;
    params.t0 = 0.0;
    params.t1 = 5.0;
    params.derivative = decay(lambda);
    dVec y0 = {1.0};
    params.initial_conditions = y0;

    std::cout << "Solving via rk1...\n";
    SolverResults results_rk1 = rk1_solver(params);

    std::cout << "Solving via rk2...\n";
    SolverResults results_rk2 = rk2_solver(params);

    std::cout << "Solving via rk3...\n";
    SolverResults results_rk3 = rk3_solver(params);

    std::cout << "Solving via standard rk4...\n";
    SolverResults results_rk4 = rk4_solver(params);

    std::cout << "Solving via 3/8 rk4...\n";
    SolverResults results_38 = rk4_38_solver(params);

    std::cout << "Solving via gill's rk4...\n";
    SolverResults results_gill = rk4_gill_solver(params);

    std::cout << "Solving via ralston's rk4...\n";
    SolverResults results_ralston = rk4_ralston_solver(params);

    dMatrix sol_rk1     = results_rk1.solution;
    dMatrix sol_rk2     = results_rk2.solution;
    dMatrix sol_rk3     = results_rk3.solution;
    dMatrix sol_rk4     = results_rk4.solution;
    dMatrix sol_38      = results_38.solution;
    dMatrix sol_gill    = results_gill.solution;
    dMatrix sol_ralston = results_ralston.solution;

    dVec times = results_rk1.time_points;

    size_t N = y0.size();
    size_t n = times.size();

    std::cout << "Calculating exact solution...\n";
    dMatrix exact(n, dVec(N,0.0));
    for ( size_t i = 0; i < n; ++i)
    {
        exact[i] = exact_solution(lambda,times[i],y0);
    }

    std::cout << "Preparing to write to file...\n";
    dMatrix output = dMatrix( n, dVec( N*8 + 1, 0.0 ) );
    for ( size_t i = 0; i < n; ++i )
    {
        for (size_t j = 0; j < N; ++j)
        {
            output[i][j] = sol_rk1[i][j];
            output[i][j + N] = sol_rk2[i][j];
            output[i][j + 2*N] = sol_rk3[i][j];
            output[i][j + 3*N] = sol_rk4[i][j];
            output[i][j + 4*N] = sol_38[i][j];
            output[i][j + 5*N] = sol_gill[i][j];
            output[i][j + 6*N] = sol_ralston[i][j];
            output[i][j + 7*N] = exact[i][j];
        }
        output[i][8*N] = times[i];
    }

    sVec header_vector = {"rk1","rk2","rk3","rk4","3/8","gill","ralston","exact solution","time"};

    // Output directory (current directory)
    std::string out_dir = ".";

    // Parameters for adjacency matrices write function
    std::string         filename = "rk-methods-colclusion";
    std::string         filetype = "tsv";
    std::string        separator = "\t";
    std::string          comment = "";
    std::string        precision = "16";
    std::string           format = "fixed";
    std::string        alignment = "right";
    std::string           header = "rk methods' comparison!";
    std::string           footer = "All set!";
    bool                  append = false;
    size_t             col_width = 20;
    bool skip_trailing_separator = false;
    bool                  binary = false;

    std::cout << "Writing to file...\n";

    comment = "Solutions with Parameters: t0 = " + std::to_string(params.t0) + ", t1 = " + std::to_string(params.t1) + ", dt = " + std::to_string(params.dt) + ", y0 = " + std::to_string(y0[0]);
    footer  = "\n";
    write_vector_to_file(header_vector, out_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    header  = "";
    comment = "";
    append  = true;
    footer  = "All set!";
    write_matrix_to_file(output, out_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    
    std::cout << "All done!\n";
    return 0;
}

// > g++ -O3 -std=c++23 rk-solvers.cpp -o rk-solvers.exe
// > ./rk-solvers.exe