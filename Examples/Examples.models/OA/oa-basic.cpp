#include "../../../includes/initializers/initials.hpp"
#include "../../../includes/models/kuramoto/special.hpp"
#include "../../../includes/models/kuramoto/order.hpp"
#include "../../../includes/models/ott-antonsen.dim-red/OA-basic.hpp"
#include "../../../includes/network/topology.hpp"
#include "../../../includes/solvers/ODE/rk/explicit/rk4-solver.hpp"
#include "../../../includes/solvers/ODE/rk/explicit/rk4-variants.hpp"
#include "../../../includes/solvers/ODE/multistep/ab-solver.hpp"
#include "../../../includes/solvers/ODE/multistep/abm-solver.hpp"
#include "../../../includes/utility/write.hpp"

#include <chrono>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

int main()
{
    size_t num_modules = 2;
    double mu1         = 0.1;
    double mu2         = 0.2;
    double gamma1      = 0.2;
    double gamma2      = 0.2;
    double inter_K     = 1.0;
    double intra_K     = 0.5;
    double inter_dK    = 0.02;
    size_t num_inter_K = 75;
    size_t seed        = 321;

    ott_antonsen OA_params;
    OA_params.mus         = {mu1, mu2};
    OA_params.gammas      = {gamma1, gamma2};
    OA_params.connections = {{intra_K, inter_K},
                             {inter_K, intra_K}};
    dVec rho = random_uniform(2, 0.0, 1.0e-10, seed);
    dVec phi = random_uniform(1, -PI, PI, seed+1);
    phi.push_back(phi[0] + PI);
    dVec state = {rho[0]*cos(phi[0]), rho[0]*sin(phi[0]), rho[1]*cos(phi[1]), rho[1]*sin(phi[1])};

    // std::cout << "Omega: \n";
    // print_vector(omega);
    // std::cout << "Theta (initial): \n";
    // print_vector(theta0);

    // std::cout << "Threads = " << std::thread::hardware_concurrency() << '\n';
    MyFunc OA = [&](const double time, const dVec& theta)
    {
        return ott_antonsen_basic(time, theta, OA_params);
    };
    
    SolverParameters params;
    params.derivative = OA;
    params.initial_conditions = state;
    params.t0 = 0.0;
    params.t1 = 1000.0;
    params.dt = 1.0/16.0;

    std::string base_dir = "two-modules";
    fs::create_directories(base_dir);

    std::ofstream out_file;
    out_file.open("two-modules-timing.txt", std::ios::trunc);

    std::string         filename = "general";
    std::string         filetype = "tsv";
    std::string        separator = "\t";
    std::string          comment = "";
    std::string        precision = "16";
    std::string           format = "fixed";
    std::string        alignment = "left";
    std::string           header = "Sin-Comp-1\tCos-Comp-1\tMagnitude-1\tSin-Comp-2\tCos-Comp-2\tMagnitude-2\tSin-Comp-Total\tCos-Comp-Total\tMagnitude-Total\tTime";
    std::string           footer = "";
    bool                  append = false;
    size_t             col_width = 20;
    bool skip_trailing_separator = true;
    bool                  binary = false;

    auto start_time = std::chrono::system_clock::now();
    std::cout << "rk4 method...\n";
    out_file << "rk4 method...\n";
    SolverResults result_rk4 = rk4_solver(params);
    filename = "rk4";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(result_rk4.solution, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    auto end_time = std::chrono::system_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    start_time = std::chrono::system_clock::now();
    std::cout << "rk4 3/8 method...\n";
    out_file << "rk4 3/8 method...\n";
    SolverResults result_38  = rk4_38_solver(params);
    filename = "rk4-38";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(result_38.solution, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);    
    end_time = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    start_time = std::chrono::system_clock::now();
    std::cout << "rk4 gill method...\n";
    out_file << "rk4 gill method...\n";
    SolverResults result_gill = rk4_gill_solver(params);
    filename = "rk4-gill";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(result_gill.solution, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    end_time = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    start_time = std::chrono::system_clock::now();
    std::cout << "rk4 ralston method...\n";
    out_file << "rk4 ralston method...\n";
    SolverResults result_ralston = rk4_ralston_solver(params);
    filename = "rk4-ralston";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(result_ralston.solution, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    end_time = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    start_time = std::chrono::system_clock::now();
    std::cout << "adams-bashforth method...\n";
    out_file << "adams-bashforth method...\n";
    SolverResults result_ab = adams_bashforth_solver(params);
    filename = "ab";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(result_ab.solution, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    end_time = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    start_time = std::chrono::system_clock::now();
    std::cout << "adams-bashforth-moulton PECE method...\n";
    out_file << "adams-bashforth-moulton PECE method...\n";
    params.iterations = 1;
    SolverResults result_abm1 = adams_bashforth_moulton_solver(params);
    filename = "abm1";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(result_abm1.solution, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    end_time = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    start_time = std::chrono::system_clock::now();
    std::cout << "adams-bashforth-moulton PE(CE)^2 method...\n";
    out_file << "adams-bashforth-moulton PE(CE)^2 method...\n";
    params.iterations = 2;
    SolverResults result_abm2 = adams_bashforth_moulton_solver(params);
    filename = "abm2";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(result_abm2.solution, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    end_time = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    out_file.close();
}

// > g++ -O3 -std=c++23 oa-basic.cpp -o oa-basic.exe
// > ./oa-basic.exe
