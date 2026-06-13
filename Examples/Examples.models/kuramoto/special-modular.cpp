#include "../../../includes/initializers/initials.hpp"
#include "../../../includes/models/kuramoto/special.hpp"
#include "../../../includes/models/kuramoto/order.hpp"
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
    size_t N           = 500;
    size_t num_modules = 2;
    double mu1         = 0.1;
    double mu2         = 0.2;
    double gamma1      = 0.2;
    double gamma2      = 0.2;
    double inter_K     = 1.0;
    double intra_K     = 0.5;
    double inter_dK    = 0.005;
    size_t num_inter_K = 100;
    size_t seed        = 321;
    double amplitude   = 1.0 / static_cast<double>(N) * 1e-5;
    auto   theta0      = identical_modules(N, num_modules, "splay_perturbed", amplitude, 0.0, seed);
    auto   omega       = dVec(N*num_modules);
    auto   omega1      = random_cauchy(N, mu1, gamma1, seed+1);
    auto   omega2      = random_cauchy(N, mu2, gamma2, seed+2);
    size_t counter     = 0;
    for ( auto [o1, o2] : std::views::zip(omega1, omega2))
    {
        omega[counter]   = o1;
        omega[counter+N] = o2;
        ++counter;
    }
    // std::cout << "Omega: \n";
    // print_vector(omega);
    // std::cout << "Theta (initial): \n";
    // print_vector(theta0);

    // std::cout << "Threads = " << std::thread::hardware_concurrency() << '\n';
    MyFunc kuramoto = [&](const double time, const dVec& theta)
    {
        return kuramoto_special_modular_parallel(time, theta, omega, intra_K, inter_K, 0.0, N);
    };
    
    SolverParameters params;
    params.derivative = kuramoto;
    params.initial_conditions = theta0;
    params.t0 = 0.0;
    params.t1 = 1000.0;
    params.dt = 1.0/16.0;

    std::string base_dir = "special-modular";
    fs::create_directories(base_dir);

    std::ofstream out_file;
    out_file.open("special-modular-timing.txt", std::ios::trunc);

    wVec module_assignment(N*num_modules,0);
    for ( size_t i = 0; i < N; ++i)
    {
        for ( size_t j = 0; j < num_modules; ++j)
        {
            module_assignment[i+j*N] = j;
        }
    }

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
    std::cout << "Calculating order...\n";
    out_file << "Calculating order...\n";
    auto order_rk4 = calculate_order_per_module(result_rk4,module_assignment,num_modules);
    filename = "rk4";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(order_rk4, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    auto end_time = std::chrono::system_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    start_time = std::chrono::system_clock::now();
    std::cout << "rk4 3/8 method...\n";
    out_file << "rk4 3/8 method...\n";
    SolverResults result_38  = rk4_38_solver(params);
    std::cout << "Calculating order...\n";
    out_file << "Calculating order...\n";
    auto order_rk4_38 = calculate_order_per_module(result_38,module_assignment,num_modules);
    filename = "rk4-38";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(order_rk4_38, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);    
    end_time = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    start_time = std::chrono::system_clock::now();
    std::cout << "rk4 gill method...\n";
    out_file << "rk4 gill method...\n";
    SolverResults result_gill = rk4_gill_solver(params);
    std::cout << "Calculating order...\n";
    out_file << "Calculating order...\n";
    auto order_rk4_gill = calculate_order_per_module(result_gill,module_assignment,num_modules);
    filename = "rk4-gill";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(order_rk4_gill, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    end_time = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    start_time = std::chrono::system_clock::now();
    std::cout << "rk4 ralston method...\n";
    out_file << "rk4 ralston method...\n";
    SolverResults result_ralston = rk4_ralston_solver(params);
    std::cout << "Calculating order...\n";
    out_file << "Calculating order...\n";
    auto order_rk4_ralston = calculate_order_per_module(result_ralston,module_assignment,num_modules);
    filename = "rk4-ralston";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(order_rk4_ralston, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    end_time = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    start_time = std::chrono::system_clock::now();
    std::cout << "adams-bashforth method...\n";
    out_file << "adams-bashforth method...\n";
    SolverResults result_ab = adams_bashforth_solver(params);
    std::cout << "Calculating order...\n";
    out_file << "Calculating order...\n";
    auto order_ab = calculate_order_per_module(result_ab,module_assignment,num_modules);
    filename = "ab";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(order_ab, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    end_time = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    start_time = std::chrono::system_clock::now();
    std::cout << "adams-bashforth-moulton PECE method...\n";
    out_file << "adams-bashforth-moulton PECE method...\n";
    params.iterations = 1;
    SolverResults result_abm1 = adams_bashforth_moulton_solver(params);
    std::cout << "Calculating order...\n";
    out_file << "Calculating order...\n";
    auto order_abm1 = calculate_order_per_module(result_abm1,module_assignment,num_modules);
    filename = "abm1";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(order_abm1, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    end_time = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    start_time = std::chrono::system_clock::now();
    std::cout << "adams-bashforth-moulton PE(CE)^2 method...\n";
    out_file << "adams-bashforth-moulton PE(CE)^2 method...\n";
    params.iterations = 2;
    SolverResults result_abm2 = adams_bashforth_moulton_solver(params);
    std::cout << "Calculating order...\n";
    out_file << "Calculating order...\n";
    auto order_abm2 = calculate_order_per_module(result_abm2,module_assignment,num_modules);
    filename = "abm2";
    std::cout << "Writing to file...\n";
    out_file << "Writing to file...\n";
    write_matrix_to_file(order_abm2, base_dir, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    end_time = std::chrono::system_clock::now();
    elapsed_time = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time-start_time);
    std::cout << "It took " << elapsed_time << " (nano-seconds)...\n";
    out_file << "It took " << elapsed_time << " (nano-seconds)...\n";

    out_file.close();
}

// > g++ -O3 -std=c++23 special-modular.cpp -o special-modular.exe
// > ./special-modular.exe