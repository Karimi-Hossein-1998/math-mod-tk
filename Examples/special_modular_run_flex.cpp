#include "../includes/solvers/solver.hpp"
#include "../includes/initializers/initials.hpp"
#include "../includes/network/topology.hpp"
#include "../includes/models/kuramoto/special.hpp"
#include "../includes/models/kuramoto/order.hpp"
#include "../includes/utility/print.hpp"
#include "../includes/utility/write.hpp"
#include <chrono>
#include <filesystem>
#include <cstdlib> // for system() calls

namespace fs = std::filesystem;

// Parameter bundle to configure a modular special Kuramoto run
struct SpecialModularRunParams
{
    // sizes
    size_t N           = 500;
    size_t num_modules = 2;

    // distributions (rows are pairs {mu1,mu2} / {gamma1,gamma2})
    dMatrix mus         = {{-0.2,-0.2},{-0.2,0.2},{0.0,0.0},{0.2,0.2}};
    dMatrix gammas      = {{0.05,0.05},{0.05,0.1},{0.1,0.1}};

    // coupling
    double inter_K0    = 0.01;
    double intra_K0    = 0.1;
    double inter_dK    = 0.01;
    double intra_dK    = 0.0;    // step for intra_K sweep (0 means fixed intra_K)
    size_t num_inter_K = 100;
    size_t num_intra_K = 1;     // number of intra_K values (1 = fixed)

    // ensembles / seeds
    size_t simulations = 21;
    wVec   seeds;

    // initial condition amplitude
    double amplitude   = 0.0; // computed later from N if left zero

    // solver params
    SolverParameters solver_params;

    // filesystem
    std::string base_dir = "run";

    // threading
    size_t num_threads = 7;

    // precision / io options
    std::string file_precision = "11";
    std::string file_format    = "fixed";

    SpecialModularRunParams()
    {
        // default seed list
        for ( size_t i = 0; i < simulations; ++i ) seeds.push_back(i*321);
        if ( amplitude == 0.0 ) amplitude = 1e-5 / static_cast<double>(N);
        // default solver parameters (match original file)
        solver_params.dt = 1.0 / 16.0;
        solver_params.t0 = 0.0;
        solver_params.t1 = 50.0;
    }
};

// Contract:
// Inputs: SpecialModularRunParams const& params
// Outputs: writes order-parameter files into params.base_dir/<param_dir>/
// Error modes: throws on filesystem failures or runtime exceptions from solvers

// Flexible run function. It mirrors behavior in `special_modular_run.cpp` but
// accepts all tunable parameters via the params struct. It's written to be
// efficient: preallocates buffers, reuses solver parameter objects, and
// parallelises across ensemble simulations.
void run_special_modular(const SpecialModularRunParams& params_in)
{
    auto params = params_in; // make a local copy we can modify

    std::cout << "Initializing the flexible simulations!\n";

    // unpack commonly used values
    const size_t N           = params.N;
    const size_t num_modules = params.num_modules;
    const dMatrix mus         = params.mus;
    const dMatrix gammas      = params.gammas;
    const double inter_dK    = params.inter_dK;
    const size_t num_inter_K = params.num_inter_K;
    const double intra_dK    = params.intra_dK;
    const size_t num_intra_K = params.num_intra_K;
    const size_t simulations = params.simulations;
    const size_t num_threads = params.num_threads;

    // build inter_K vector
    dVec inter_Ks;
    inter_Ks.reserve(num_inter_K);
    for ( size_t i = 0; i < num_inter_K; ++i ) inter_Ks.push_back(params.inter_K0 + i*inter_dK);

    // build intra_K vector (support fixed intra_K or a sweep)
    dVec intra_Ks;
    if ( num_intra_K <= 1 )
    {
        intra_Ks.push_back(params.intra_K0);
    }
    else
    {
        intra_Ks.reserve(num_intra_K);
        for ( size_t i = 0; i < num_intra_K; ++i ) intra_Ks.push_back(params.intra_K0 + i*intra_dK);
    }

    // prepare seeds if not supplied
    wVec seeds = params.seeds;
    if ( seeds.size() < simulations )
    {
        seeds.clear();
        for ( size_t i = 0; i < simulations; ++i ) seeds.push_back(i*321);
    }

    // initial conditions
    dVec theta0(N*num_modules,0.0);
    dMatrix theta0s(simulations,theta0);
    for ( size_t i = 0; i < simulations; ++i )
    {
        theta0s[i] = identical_modules(N,num_modules,"splay_perturbed",params.amplitude,0.0,seeds[i]);
    }

    // omega arrays (preallocated)
    dMatrix omegas(simulations,dVec(N*num_modules,0.0));

    // module assignment
    wVec module_Assignment(N*num_modules,0);
    for ( size_t j = 0; j < num_modules; ++j)
    {
        for ( size_t i = 0; i < N; ++i)
        {
            module_Assignment[i+j*N] = j;
        }
    }
    const wVec module_assignment = module_Assignment;

    // header for output files
    std::string header = "";
    for ( size_t i = 0; i < num_modules; i++)
    {
        header += "Sin comp.\tCos comp.\tMagnitude\t";
    }
    header += "Sin total.\tCos total.\tMagnitude\tTime";

    // prepare output directories
    const std::string base_dir = params.base_dir;
    fs::create_directories(base_dir);
    std::ofstream dirs("directories.txt", std::ios::out | std::ios::trunc);

    // include intra_K sweep in total iterations for progress estimation
    size_t total_iterations_parallel = mus.size() * gammas.size() * intra_Ks.size();
    size_t current = 0;

    auto start_time = std::chrono::system_clock::now();

    std::cout << "Starting the simulations ...\n";

    for ( auto intra_K : intra_Ks )
    {
        // create intra_K level directory
        std::string intra_dir = base_dir + "/intra_K_" + std::to_string(intra_K);
        std::replace(intra_dir.begin(), intra_dir.end(), '.', '_');
        fs::create_directories(intra_dir);

        for ( auto mu12 : mus )
        {
            for ( auto gamma12 : gammas )
            {
                const double mu1    = mu12[0];
                const double mu2    = mu12[1];
                const double gamma1 = gamma12[0];
                const double gamma2 = gamma12[1];

                std::string dir_name = intra_dir + "/mu1_" + std::to_string(mu1) + "_mu2_" + std::to_string(mu2) +
                                       "_gamma1_" + std::to_string(gamma1) + "_gamma2_" + std::to_string(gamma2);
                std::replace(dir_name.begin(), dir_name.end(), '.', '_');
                dirs << dir_name << '\n';
                fs::create_directories(dir_name);

                // prepare omegas for all simulations in this parameter set
                for ( size_t i = 0; i < simulations; ++i )
                {
                    dVec omega1 = random_cauchy(N, mu1, gamma1, seeds[i]+1);
                    dVec omega2 = random_cauchy(N, mu2, gamma2, seeds[i]+2);
                    size_t counter = 0;
                    for ( auto [o1,o2] : std::views::zip(omega1,omega2) )
                    {
                        omegas[i][counter]   = o1;
                        omegas[i][counter+N] = o2;
                        ++counter;
                    }
                }

                // parallel loop over simulations
                size_t chunk_size = std::max<size_t>(1, simulations / num_threads);
                Vec<std::jthread> threads;
                for ( size_t thread_idx = 0; thread_idx < num_threads; ++thread_idx )
                {
                    threads.emplace_back([&,thread_idx]()
                    {
                        size_t start_idx = thread_idx * chunk_size;
                        size_t end_idx   = (thread_idx == num_threads - 1) ? simulations : (thread_idx + 1) * chunk_size;
                        for ( size_t i = start_idx; i < end_idx; ++i )
                        {
                            auto local_params = params.solver_params; // copy, we will set initial conditions & derivative per run
                            local_params.initial_conditions = theta0s[i];
                            size_t j = 0;
                            for ( auto inter_K : inter_Ks )
                            {
                                auto f = [&](const double time, const dVec& theta)
                                {
                                    return kuramoto_special_modular(time, theta, omegas[i], intra_K, inter_K, 0.0, N);
                                };
                                local_params.derivative = f;
                                auto results = adams_bashforth_solver(local_params);

                                auto orders = calculate_order_per_module(results,module_assignment,num_modules);

                                write_matrix_to_file(
                                    orders,
                                    dir_name,
                                    "order_parameter_"+std::to_string(i)+"_"+std::to_string(j),
                                    "tsv",
                                    "\t",
                                    "",
                                    params.file_precision,
                                    params.file_format,
                                    "",
                                    header
                                );
                                ++j;
                            }
                        }
                    });
                }

                ++current;
                auto now = std::chrono::system_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>( now - start_time );
                auto eta = elapsed * ( total_iterations_parallel - current ) / current;
                print_progress_bar_eta(current, total_iterations_parallel, eta, 48);
            }
        }
    }

    dirs.close();

    auto end_time = std::chrono::system_clock::now();
    auto total_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>( end_time - start_time ).count();
    std::cout << "Total time (ns): " << total_elapsed << "\n";
}

// Small driver that mirrors original `main()` but uses the flexible function
int main()
{
    SpecialModularRunParams params;
    // keep defaults but ensure amplitude matches original style
    params.amplitude = 1.0 / static_cast<double>(params.N) * 1e-5;
    
    params.intra_dK = 0.1;
    params.num_intra_K = 7;

    // call flexible runner
    run_special_modular(params);
    return 0;
}
