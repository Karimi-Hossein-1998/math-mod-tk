#include "../../../includes/solvers/ODE/multistep/ab-solver.hpp"
#include "../../../includes/solvers/ODE/multistep/abm-solver.hpp"
#include "../../../includes/utility/write.hpp"
#include "../../../includes/solvers/ODE/adaptive/adaptive-custom.hpp"

inline dVec unwrapped_decay(double lambda, double t, const dVec &y)
{
    size_t  n = y.size();
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
    params.dt                 = 1.0 / 16.0;
    params.t0                 = 0.0;
    params.t1                 = 5.0;
    params.derivative         = decay(lambda);
    dVec y0                   = {1.0};
    params.initial_conditions = y0;

    // Adaptive-special arguments
    params.local_tol             = 1.0e-03;
    params.absolute_tol          = 1.0e-010;
    params.local_tol_error_ratio = 0.0001;
    params.attempts_history      = true;
    params.decrease_factor       = 0.9;
    params.increase_factor       = 1.1;
    params.max_dt                = 0.5;
    params.min_dt                = 1.0 / 128.0;
    params.error_estimate        = true;
    params.variable_steps        = true;
    params.norm_error            = true;
    params.max_trial             = 25;

    std::cout << "Solving via rk12...\n";
    SolverResults results_rk12 = rk12_custom_solver(params);

    std::cout << "Solving via rk23...\n";
    SolverResults results_rk23 = rk23_custom_solver(params);

    std::cout << "Solving via rk34...\n";
    SolverResults results_rk34 = rk34_custom_solver(params);

    std::cout << "Solving via rk45...\n";
    SolverResults results_rk45 = rk45_custom_solver(params);

    dMatrix sol_rk12 = results_rk12.solution;
    dMatrix sol_rk23 = results_rk23.solution;
    dMatrix sol_rk34 = results_rk34.solution;
    dMatrix sol_rk45 = results_rk45.solution;

    dVec times12 = results_rk12.time_points;
    dVec times23 = results_rk23.time_points;
    dVec times34 = results_rk34.time_points;
    dVec times45 = results_rk45.time_points;
    dVec times(static_cast<size_t>((params.t1-params.t0)/params.dt) + 1,0.0);
    for ( size_t i = 0; i < times.size(); ++i)
    {
        times[i] = params.t0 + i * params.dt;
    }

    size_t N = y0.size();
    wVec sizes = {times12.size(),times23.size(),times34.size(),times45.size(),times.size()};
    dVec dsizes = dVec(sizes.size(),0.0);
    for ( size_t i = 0; i < sizes.size(); ++i)
    {
        dsizes[i] = static_cast<double>(sizes[i]);
    }

    size_t n = *std::max_element(dsizes.begin(),dsizes.end());

    std::cout << "Calculating exact solution...\n";
    dMatrix exact(n, dVec(N,0.0));
    for ( size_t i = 0; i < n; ++i)
    {
        exact[i] = exact_solution(lambda,times[i],y0);
    }

    std::cout << "Preparing to write to file...\n";
    dMatrix output = dMatrix( n, dVec( N*5 + 5, 0.0 ) );
    for ( size_t i = 0; i < n; ++i )
    {
        for (size_t j = 0; j < N; ++j)
        {
            if ( i < sol_rk12.size())
            {
                output[i][j] = sol_rk12[i][j];
            }
            else
            {
                output[i][j] = 0.0;
            }
            if ( i < sol_rk23.size())
            {
                output[i][j+N] = sol_rk23[i][j];
            }
            else
            {
                output[i][j+N] = 0.0;
            }
            if ( i < sol_rk34.size())
            {
                output[i][j+2*N] = sol_rk34[i][j];
            }
            else
            {
                output[i][j+2*N] = 0.0;
            }
            if ( i < sol_rk45.size())
            {
                output[i][j+3*N] = sol_rk45[i][j];
            }
            else
            {
                output[i][j+3*N] = 0.0;
            }
            if ( i < times.size())
            {
                output[i][j + 4*N] = exact[i][j];
            }
            else
            {
                output[i][j + 4*N] = 0.0;
            }
        }
        if ( i < times12.size())
        {
            output[i][5*N] = times12[i];
        }
        else
        {
            output[i][5*N] = params.t1;
        }
        if ( i < times23.size())
        {
            output[i][5*N+1] = times23[i];
        }
        else
        {
            output[i][5*N+1] = params.t1;
        }
        if ( i < times34.size())
        {
            output[i][5*N+2] = times34[i];
        }
        else
        {
            output[i][5*N+2] = params.t1;
        }
        if ( i < times45.size())
        {
            output[i][5*N+3] = times45[i];
        }
        else
        {
            output[i][5*N+3] = params.t1;
        }
        if ( i < times.size())
        {
            output[i][5*N+4] = times[i];
        }
        else
        {
            output[i][5*N+4] = params.t1;
        }
    }

    sVec header_vector = {"rk12","rk23","rk34","rk45","exact solution","time12","time23","time34","time45","time"};

    // Output directory (current directory)
    std::string out_dir = ".";

    // Parameters for adjacency matrices write function
    std::string         filename = "custom-adaptive-methods-comparison";
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

    append   = false;
    filename = "rk12-errors";
    header   = "Error History.";
    dMatrix rk12_err = results_rk12.errors_history;
    write_matrix_to_file(rk12_err, out_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    append = true;
    header = "Steps History.";
    dMatrix rk12_steps = results_rk12.steps_history;
    write_matrix_to_file(rk12_steps, out_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    header = "Fails.";
    wVec rk12_fails = results_rk12.failed_trials;
    write_vector_to_file(rk12_fails, out_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    std::cout << "All done!\n";
    return 0;
}

// > g++ -O3 -std=c++23 adaptive-custom.cpp -o adaptive-custom.exe
// > ./adaptive-custom.exe