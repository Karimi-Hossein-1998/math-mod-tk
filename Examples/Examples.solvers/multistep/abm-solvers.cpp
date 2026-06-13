#include "../../../includes/solvers/ODE/multistep/ab-solver.hpp"
#include "../../../includes/solvers/ODE/multistep/abm-solver.hpp"
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
    params.order = 4;

    std::cout << "Solving via Adams-Bashforth (explicit)...\n";
    SolverResults results_ab = adams_bashforth_solver(params);

    std::cout << "Solving via Adams-Bashforth-Moulton PE(CE)...\n";
    params.iterations = 1;
    SolverResults results_abm1 = adams_bashforth_moulton_solver(params);

    std::cout << "Solving via Adams-Bashforth-Moulton PE(CE)^2...\n";
    params.iterations = 2;
    SolverResults results_abm2 = adams_bashforth_moulton_solver(params);

    std::cout << "Solving via Adams-Bashforth-Moulton PE(CE)^3...\n";
    params.iterations = 3;
    SolverResults results_abm3 = adams_bashforth_moulton_solver(params);

    std::cout << "Solving via Adams-Bashforth-Moulton PE(CE)^4...\n";
    params.iterations = 4;
    SolverResults results_abm4 = adams_bashforth_moulton_solver(params);

    std::cout << "Solving via Adams-Bashforth-Moulton PE(CE)^5...\n";
    params.iterations = 5;
    SolverResults results_abm5 = adams_bashforth_moulton_solver(params);

    std::cout << "Solving via Adams-Bashforth-Moulton PE(CE)^6...\n";
    params.iterations = 6;
    SolverResults results_abm6 = adams_bashforth_moulton_solver(params);

    dMatrix sol_ab     = results_ab.solution;
    dMatrix sol_abm     = results_abm1.solution;
    dMatrix sol_abm2     = results_abm2.solution;
    dMatrix sol_abm3     = results_abm3.solution;
    dMatrix sol_abm4      = results_abm4.solution;
    dMatrix sol_abm5    = results_abm5.solution;
    dMatrix sol_abm6 = results_abm6.solution;

    dVec times = results_ab.time_points;

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
            output[i][j] = sol_ab[i][j];
            output[i][j + N] = sol_abm[i][j];
            output[i][j + 2*N] = sol_abm2[i][j];
            output[i][j + 3*N] = sol_abm3[i][j];
            output[i][j + 4*N] = sol_abm4[i][j];
            output[i][j + 5*N] = sol_abm5[i][j];
            output[i][j + 6*N] = sol_abm6[i][j];
            output[i][j + 7*N] = exact[i][j];
        }
        output[i][8*N] = times[i];
    }

    sVec header_vector = {"ab","abm1","abm2","abm3","abm4","abm5","abm6","exact solution","time"};

    // Output directory (current directory)
    std::string out_dir = ".";

    // Parameters for adjacency matrices write function
    std::string         filename = "abm-methods-colclusion_order-"+std::to_string(params.order);
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

// > g++ -O3 -std=c++23 abm-solvers.cpp -o abm-solvers.exe
// > ./abm-solvers.exe