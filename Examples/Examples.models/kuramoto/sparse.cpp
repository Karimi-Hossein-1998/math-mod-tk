#include "../../../includes/initializers/initials.hpp"
#include "../../../includes/models/kuramoto/sparse.hpp"
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
    size_t   N     = 1000;
    double   K     = 1.0;
    double   alpha = 0.0;
    unsigned seed  = 42;
    
    // Generate random initial phases and frequencies
    dVec theta0 = random_uniform(N, -PI, PI, seed);
    dVec omega  = random_normal(N, 0.0, 1.0, seed+1);
    
    // Generate a random dense adjacency matrix
    SparsedMatrix adj = small_world_sparse(N, 40, 0.5, 1.0, seed+2);
    // Define the Kuramoto ODE
    MyFunc kuramoto = [&](double t, const dVec& theta) 
    {
        return kuramoto_sparse(t, theta, omega, K, adj, alpha);
    };
    SolverParameters params;
    params.derivative = kuramoto;
    params.initial_conditions = theta0;
    params.t0 = 0.0;
    params.t1 = 1000.0;
    params.dt = 1.0/16.0;

    std::string base_dir = "sparse";
    fs::create_directories(base_dir);

    std::ofstream out_file;
    out_file.open("sparse-timing.txt", std::ios::trunc);

    std::string         filename = "general";
    std::string         filetype = "txt";
    std::string        separator = " ";
    std::string          comment = "";
    std::string        precision = "16";
    std::string           format = "fixed";
    std::string        alignment = "left";
    std::string           header = "Sin-Comp-1 Cos-Comp-1 Magnitude-1 Time";
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
    auto order_rk4 = calculate_order(result_rk4);
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
    auto order_rk4_38 = calculate_order(result_38);
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
    auto order_rk4_gill = calculate_order(result_gill);
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
    auto order_rk4_ralston = calculate_order(result_ralston);
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
    auto order_ab = calculate_order(result_ab);
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
    auto order_abm1 = calculate_order(result_abm1);
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
    auto order_abm2 = calculate_order(result_abm2);
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

// > g++ -O3 -std=c++23 sparse.cpp -o sparse.exe
// > ./sparse.exe