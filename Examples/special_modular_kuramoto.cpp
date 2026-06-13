#include "../includes/solvers/solver.hpp"
#include "../includes/initializers/initials.hpp"
#include "../includes/network/topology.hpp"
#include "../includes/models/kuramoto/special.hpp"
#include "../includes/models/kuramoto/order.hpp"
#include "../includes/utility/print.hpp"
#include "../includes/utility/write.hpp"
#include <chrono>

int main()
{
    auto begin_time    = std::chrono::system_clock::now();
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
    auto f = [&](const double time, const dVec& theta)
    {
        return kuramoto_special_modular_parallel(time, theta, omega, intra_K, inter_K, 0.0, N);
    };
    auto params               = SolverParameters{};
    params.dt                 = 1.0/16.0;
    params.initial_conditions = theta0;
    params.derivative         = f;
    params.t0                 = 0.0;
    params.t1                 = 200.0;

    // Solve the system
    std::fstream tp("timing.txt", std::ios::out | std::ios::trunc);
    tp << std::setprecision(15);
    tp << "Number of available threads: " << std::thread::hardware_concurrency() << '\n';
    tp << "Testing PARALLEL vs SEQUENTIAL model functions:\nPARALLEL MODE!\n";
    tp << "Solving Kuramoto model..." << std::endl;
    auto start_time   = std::chrono::system_clock::now(); 
    auto results      = adams_bashforth_solver(params);    
    auto end_time     = std::chrono::system_clock::now();
    auto time_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>( end_time - start_time );
    tp << "Time elapsed: " << time_elapsed << '\n';

    // Calculate order parameter evolution
    tp << "Calculating order parameter..." << std::endl;
    start_time   = std::chrono::system_clock::now();
    auto order   = calculate_order(results);
    end_time     = std::chrono::system_clock::now();
    time_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>( end_time - start_time );
    tp << "Time elapsed: " << time_elapsed << '\n';
    tp << "Writing Order to file!\n";
    
    // write to file...
    write_matrix_to_file(
        order,
        ".", // directory: current directory (Examples)
        "order_parameter", // filename
        "tsv", // filetype
        "\t", // separator (space)
        "", // comment
        "6", // precision (to match setprecision(6) in terminal)
        "fixed", // format
        "", // alignment
        "Parallel\nSin comp.\tCos comp.\tMagnitude\tTime", // header
        std::string(60, '-'), // footer
        false // append
    );

    // std::cout << "Threads = " << std::thread::hardware_concurrency() << '\n';
    auto g = [&](const double time, const dVec& theta)
    {
        return kuramoto_special_modular(time, theta, omega, intra_K, inter_K, 0.0, N);
    };
    params.derivative = g;
    // Solve the system
    tp << "\nSEQUENTIAL MODE!\n";
    tp << "Solving Kuramoto model..." << std::endl;
    start_time   = std::chrono::system_clock::now();
    results      = adams_bashforth_solver(params);    
    end_time     = std::chrono::system_clock::now();
    time_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>( end_time - start_time );
    tp << "Time elapsed: " << time_elapsed << '\n';

    // Calculate order parameter evolution
    tp << "Calculating order parameter..." << std::endl;
    start_time   = std::chrono::system_clock::now();
    order        = calculate_order(results);
    end_time     = std::chrono::system_clock::now();
    time_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>( end_time - start_time );
    tp << "Time elapsed: " << time_elapsed << '\n';
    tp << "Writing Order to file!\n";

    // write to file...
    write_matrix_to_file(
        order,
        ".", // directory: current directory (Examples)
        "order_parameter", // filename
        "tsv", // filetype
        "\t", // separator ('\t')
        "", // comment
        "6", // precision (to match setprecision(6) in terminal)
        "fixed", // format
        "", // alignment
        "Sequential\nSin comp.\tCos comp.\tMagnitude\tTime", // header
        std::string(60, '-'), // footer
        true // append
    );

    auto finish_time   = std::chrono::system_clock::now();
    auto total_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>( finish_time - begin_time );
    tp << "Done!\n";
    return 0;
}