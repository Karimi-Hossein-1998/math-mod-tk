#include "../includes/solvers/solver.hpp"
#include "../includes/initializers/initials.hpp"
#include "../includes/network/topology.hpp"
#include "../includes/models/kuramoto/special.hpp"
#include "../includes/models/kuramoto/order.hpp"
#include "../includes/utility/print.hpp"
#include "../includes/utility/write.hpp"
#include <chrono>
#include <filesystem>
#include <cstdlib> // for **system()** calls.


namespace fs = std::filesystem;

int main()
{
    std::cout << "Initializing the simulations!\n";
    std::string output = "Simulation details:\n";
    size_t N           = 500;
    size_t num_modules = 2;
    dMatrix mus         = {{-0.2,-0.2},{-0.2,0.2},{0.0,0.0},{0.2,0.2}};
    double mu1         = mus[0][0];
    double mu2         = mus[0][1];
    dMatrix gammas      = {{0.05,0.05},{0.05,0.1},{0.1,0.1}};
    double gamma1      = gammas[0][0];
    double gamma2      = gammas[0][1];
    double inter_K0    = 0.01;
    double intra_K0    = 0.1;
    double inter_dK    = 0.01;
    dVec   inter_Ks;
    size_t num_inter_K = 100;
    for (size_t i = 0; i < num_inter_K; ++i)
    {
        inter_Ks.push_back(inter_K0 + i*inter_dK);
    }
    size_t simulations = 21;
    wVec   seeds;
    for ( size_t i = 0; i < simulations; ++i)
    {
        seeds.push_back(i*321);
    }
    double amplitude = 1.0 / static_cast<double>(N) * 1e-5;
    dVec   theta0(N*num_modules,0.0);
    dMatrix theta0s(simulations,theta0);
    dVec   omega1(N,0.0);
    dVec   omega2(N,0.0);
    dMatrix omegas(simulations,dVec(N*num_modules,0.0));
    size_t counter = 0;
    for ( size_t i = 0; i < simulations; ++i)
    {
        theta0s[i] = identical_modules(N,num_modules,"splay_perturbed",amplitude,0.0,seeds[i]);
    }

    // Parallelisation Initialisation
    // size_t num_threads = std::min(simulations, static_cast<size_t>(std::max(4u, std::thread::hardware_concurrency())));
    size_t num_threads = 7;
    std::cout << "Number of threads = " << num_threads << '\n';
    size_t chunk_size = simulations / num_threads;
    std::cout << "Chunk size = " << chunk_size << '\n';
    // Add simulation parameters to output
    output += "N = " + std::to_string(N) + "\n";
    output += "num_modules = " + std::to_string(num_modules) + "\n";
    output += "mus = {mu1,mu2} = {";
    for (const auto& mu : mus) output += "{" + std::to_string(mu[0]) + "," + std::to_string(mu[1]) + "} ";
    output += "}\n";
    output += "gammas = {gamma1,gamma2} = {";
    for (const auto& gamma : gammas) output += "{" + std::to_string(gamma[0]) + "," + std::to_string(gamma[1]) + "} ";
    output += "}\n";
    output += "inter_K0 = " + std::to_string(inter_K0) + ", intra_K0 = " + std::to_string(intra_K0) + ", inter_dK = " + std::to_string(inter_dK) + "\n";
    output += "num_inter_K = " + std::to_string(num_inter_K) + ", simulations = " + std::to_string(simulations) + "\n";
    std::ostringstream amp_stream;
    amp_stream << std::setprecision(10) << amplitude;
    output += "amplitude = " + amp_stream.str() + "\n";

    auto params0 = SolverParameters{};
    params0.dt = 1.0 / 16.0;
    params0.t0 = 0.0;
    params0.t1 = 50.0;
    wVec       module_Assignment(N*num_modules,0);
    for ( size_t i = 0; i < N; ++i)
    {
        for ( size_t j = 0; j < num_modules; ++j)
        {
            module_Assignment[i+j*N] = j;
        }
    }
    const wVec module_assignment = module_Assignment;
    std::string header = "";
    for ( size_t i = 0; i < num_modules; i++)
    {
        header += "Sin comp.\tCos comp.\tMagnitude\t";
    }
    header += "Sin total.\tCos total.\tMagnitude\tTime";

    std::string base_dir = "run";
    fs::create_directories(base_dir);
    std::ofstream dirs("directories.txt", std::ios::out | std::ios::trunc);
    size_t     total_iterations  = mus.size() * gammas.size() * inter_Ks.size() * simulations;
    size_t     current_iteration = 0;
    auto       intra_K = intra_K0;
    const auto start   = std::chrono::system_clock::now();
    output += "Step size = " + std::to_string(params0.dt) + " , Initial time = " + std::to_string(params0.t0) + " , Final time = " + std::to_string(params0.t1) + '\n';
    output += "Starting the simulations ...\n";
    std::cout << output;
    size_t total_iterations_parallel = mus.size() * gammas.size();

    auto Orders_simulations = Vec<Vec<dMatrix>>(simulations, Vec<dMatrix>(num_inter_K));
    std::ofstream checking("checking.txt", std::ios::out | std::ios::trunc);
    size_t current = 0;
    for ( auto mu12 : mus)
    {
        for ( auto gamma12 : gammas)
        {
            mu1     = mu12[0];
            mu2     = mu12[1];
            gamma1  = gamma12[0];
            gamma2  = gamma12[1];
            // Create directory name based on parameters
            counter = 0;
            std::string dir_name = base_dir + "/mu1_" + std::to_string(mu1) + "_mu2_" + std::to_string(mu2) +
                                   "_gamma1_" + std::to_string(gamma1) + "_gamma2_" + std::to_string(gamma2);
            std::replace(dir_name.begin(), dir_name.end(), '.', '_'); // Replace '.' with '_' for filesystem safety
            dirs << dir_name << '\n';
            fs::create_directories(dir_name);
            output += "Created directory: " + dir_name + "\n";
            { // Begin Block
            for ( size_t i = 0; i < simulations; ++i)
            {
                omega1  = random_cauchy(N, mu1, gamma1, seeds[i]+1);
                omega2  = random_cauchy(N, mu2, gamma2, seeds[i]+2);
                counter = 0;
                for ( auto [o1, o2] : std::views::zip(omega1, omega2))
                {
                    omegas[i][counter]   = o1;
                    omegas[i][counter+N] = o2;
                    ++counter;
                }
            }        
            Vec<std::jthread> threads;
            for (size_t thread_idx = 0; thread_idx < num_threads; ++thread_idx) 
            {
                threads.emplace_back([&, thread_idx]() 
                {
                    size_t start_idx = thread_idx * chunk_size;
                    size_t end_idx   = (thread_idx == num_threads - 1) ? simulations : (thread_idx + 1) * chunk_size;
                    for ( size_t i = start_idx; i < end_idx; ++i)
                    {
                        auto params = SolverParameters{};
                        params.dt = 1.0 / 16.0;
                        params.t0 = 0.0;
                        params.t1 = 50.0;
                        params.initial_conditions = theta0s[i];
                        size_t j = 0;
                        for ( auto inter_K : inter_Ks)
                        {
                            auto f = [&](const double time, const dVec& theta)
                            {
                                return kuramoto_special_modular(time, theta, omegas[i], intra_K, inter_K, 0.0, N);
                            };
                            params.derivative        = f;
                            auto results             = adams_bashforth_solver(params);                    
                            // Orders_simulations[i][j] = calculate_order_per_module(results,module_assignment,num_modules);
                            auto orders = calculate_order_per_module(results,module_assignment,num_modules);                            
                            write_matrix_to_file(
                                orders,
                                dir_name, // directory: parameter-specific directory
                                "order_parameter_"+std::to_string(i)+"_"+std::to_string(j), // filename
                                "tsv", // filetype
                                "\t", // separator (space)
                                "", // comment
                                "6", // precision (to match setprecision(6) in terminal)
                                "fixed", // format
                                "", // alignment
                                header
                            );
                            ++j;
                        }
                    }
                });
            }
            }// End Block
            // for ( size_t i = 0; i < simulations; ++i )
            // {
            //     for ( size_t j = 0; j < num_inter_K; ++j )
            //     {
            //         write_matrix_to_file(
            //             Orders_simulations[i][j],
            //             dir_name, // directory: parameter-specific directory
            //             "order_parameter_"+std::to_string(i), // filename
            //             "tsv", // filetype
            //             "\t", // separator (space)
            //             "", // comment
            //             "6", // precision (to match setprecision(6) in terminal)
            //             "fixed", // format
            //             "", // alignment
            //             "Inter-modular coupling = " + std::to_string(inter_Ks[j]) + '\n' + header,
            //             std::string(100,'-'),
            //             true
            //         );
            //     }
            // }
            // std::string zip_command;
            // #ifndef _WIN32
            //     // Linux/Ubuntu: Use 7z with maximum compression (LZMA2, level 9)
            //     zip_command = "$HOME/use/lib/p7zip/7z a -t7z -m0=lzma2 -mx=9 -mfb=273 -md=1024m '" + dir_name + ".7z' '" + dir_name + "'";
            // #else
            //     // Windows: Use PowerShell to call 7z with maximum compression
            //     zip_command = "powershell -Command \"& 'C:\\Program Files\\7-Zip\\7z.exe' a -t7z -m0=lzma2 -mx=9 -mfb=273 -md=1024m '" + dir_name + ".7z' '" + dir_name + "'\"";
            // #endif
            // system(zip_command.c_str());
            // Remove the original directory after compression
            // fs::remove_all(dir_name);            
            checking << dir_name << ": The work is done!\n";
            auto time_now = std::chrono::system_clock::now();
            auto elapsed_this = std::chrono::duration_cast<std::chrono::nanoseconds>( time_now - start );
            ++current;
            auto eta = elapsed_this * ( total_iterations_parallel - current ) / current;
            print_progress_bar_eta(current, total_iterations_parallel, eta, 48);
        }
    }
    checking.close();
    dirs.close();
    output += "\nThe simulations are done! ... \n";
    output += "Writing filenames into the 'files.txt' ... ";
    std::cout << "\nThe simulations are done! ... \n";
    std::cout << "Writing filenames into the 'files.txt' ... ";
    std::ofstream files("files.txt", std::ios::out | std::ios::trunc);
    for ( size_t i = 0; i < simulations; ++i)
    {
        for ( auto inter_k : inter_Ks)
        {
            files << "order_parameter_"+std::to_string(i)+"_"+std::to_string(inter_k) << '\n';
        }
    }
    std::cout << "Done!\n";
    output += "Done!\n";
    files.close();
    auto end     = std::chrono::system_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>( end - start ).count();
    std::cout << "The code took " + std::to_string(elapsed) + " ms to run!\n";
    output += "The code took " + std::to_string(elapsed) + " ms to run!\n";
    // Write to time.txt
    std::ofstream time("time.txt", std::ios::out | std::ios::trunc);
    time << output;
    time.close();
    return 0;
}