#include "../../../includes/initializers/initials.hpp"
#include "../../../includes/models/kuramoto/special.hpp"
#include "../../../includes/models/kuramoto/order.hpp"
#include "../../../includes/models/ott-antonsen.dim-red/OA-basic.hpp"
#include "../../../includes/models/ott-antonsen.dim-red/OA-order.hpp"
#include "../../../includes/network/topology.hpp"
#include "../../../includes/solvers/ODE/multistep/abm-solver.hpp"
#include "../../../includes/utility/write.hpp"
#include "../../../includes/utility/print.hpp"
#include "../../../includes/statistics/statistics.hpp"
#include "../../../includes/statistics/statistics-extended.hpp"
#include "../../../includes/linalg/basics/matrix-properties.hpp"

#include <chrono>
#include <filesystem>
#include <cstdlib>

namespace fs = std::filesystem;

int main()
{
    // Setting up the base directory
    std::string base_dir = "two-modules-kq";
    fs::create_directories(base_dir);
    // Setting up the `write` arguments 
    std::string         dir_name = "";
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
    size_t             col_width = 0;
    bool skip_trailing_separator = true;
    bool                  binary = false;
    // Setting up the solver parameters
    double init_time   = 0.0;
    double final_time  = 100.0;
    double dt          = 1.0/16.0;
    size_t num_modules = 2;
    double inter_K0    = 0.01;
    double intra_K0    = 0.01;
    double inter_dK    = 0.01;
    size_t num_inter_K = 100;
    double intra_dK    = 0.01;
    size_t num_intra_K = 100;
    size_t seed        = 321;
    size_t sim_num     = 21;

    // Setting up initial conditions
    dMatrix states;
    for ( size_t i = 0; i < sim_num; ++i )
    {
        dVec rho = random_uniform(2, 0.0, 1.0e-10, seed+2*i);
        dVec phi = random_uniform(1, -PI, PI, seed+1+2*i);
        phi.push_back(phi[0] + PI);
        dVec state = {rho[0]*cos(phi[0]), rho[0]*sin(phi[0]), rho[1]*cos(phi[1]), rho[1]*sin(phi[1])};
        states.push_back(state);
    }

    // Setting up the `gammas`
    dMatrix the_gammas = {{0.1, 0.1},
                          {0.1, 0.0},
                          {0.1, -0.1},
                          {0.0, 0.0},
                          {0.0, -0.1},
                          {-0.1, -0.1}};
    // Setting up the `mus`
    dMatrix the_mus = {{0.05, 0.5},
                       {0.05, 0.0},
                       {0.05, -0.05},
                       {0.0, 0.0},
                       {0.0, -0.05},
                       {-0.05, -0.05}};

    // dMatrix the_gammas = {{0.1,0.1}};
    // dMatrix the_mus    = {{0.05,0.05}};

    // Setting up the connections (coupling strengths)
    dVec intra_Ks = dVec(num_intra_K,0.0);
    dVec inter_Ks = dVec(num_inter_K,0.0);
    for ( size_t i = 0; i < num_intra_K; ++i )
    {
        intra_Ks[i] = intra_K0 + i*intra_dK;
    }
    for ( size_t i = 0; i < num_inter_K; ++i )
    {
        inter_Ks[i] = inter_K0 + i*inter_dK;
    }

    std::string stats_header = "";
    for ( auto k : inter_Ks)
    {
        stats_header += "Q="+std::to_string(k)+"\t";
    }

    // Initializing the necessary counter variables
    size_t g12 = 0, m12 = 0, inner = 0, outer = 0, counter = 0;
    // Initializing the `SolverParameters`
    SolverParameters params;
    params.t0         = init_time;
    params.t1         = final_time;
    params.dt         = dt;
    params.iterations = 1;
    // Declaring the containers for data;
    dMatrix magnitude_dummy;
    dMatrix order_sqr_dummy;
    dMatrix order_dummy;

    size_t total_count = the_gammas.size()*the_mus.size()*num_inter_K*num_intra_K*sim_num;
    dVec   percentages = {0.01, 0.05, 0.1, 0.2};

    // Recording `start time`
    auto start_time = std::chrono::system_clock::now();

    for ( dVec mus : the_mus )
    {
        // Updating the {mu1, mu2} counter
        m12 += 1;
        // Resetting {gamma1, gamma2} counter to 0
        g12 = 0;

        for ( dVec gammas : the_gammas )
        {
            // Updating the {gamma1, gamma2} counter
            g12 += 1;
            // Resetting the inter_K (outer coupling) counter to 0
            outer = 0;

            // Creating the corresponding directory
            dir_name = base_dir 
                        + "/" + std::to_string(m12) 
                        + "_" + std::to_string(g12);
            fs::create_directories(dir_name);

            Vec<dMatrix> order1_means   = Vec(percentages.size(),dMatrix(num_intra_K, dVec(num_inter_K, 0.0)));
            Vec<dMatrix> order2_means   = Vec(percentages.size(),dMatrix(num_intra_K, dVec(num_inter_K, 0.0)));
            Vec<dMatrix> order_means    = Vec(percentages.size(),dMatrix(num_intra_K, dVec(num_inter_K, 0.0)));
            Vec<dMatrix> order1_stddevs = Vec(percentages.size(),dMatrix(num_intra_K, dVec(num_inter_K, 0.0)));
            Vec<dMatrix> order2_stddevs = Vec(percentages.size(),dMatrix(num_intra_K, dVec(num_inter_K, 0.0)));
            Vec<dMatrix> order_stddevs  = Vec(percentages.size(),dMatrix(num_intra_K, dVec(num_inter_K, 0.0)));
            Vec<dMatrix> order1_cvs     = Vec(percentages.size(),dMatrix(num_intra_K, dVec(num_inter_K, 0.0)));
            Vec<dMatrix> order2_cvs     = Vec(percentages.size(),dMatrix(num_intra_K, dVec(num_inter_K, 0.0)));
            Vec<dMatrix> order_cvs      = Vec(percentages.size(),dMatrix(num_intra_K, dVec(num_inter_K, 0.0)));
            for ( double Q : inter_Ks )
            {
                // Updating the inter_K (outer coupling) counter
                outer += 1;
                // Resetting the intra_K (inner coupling) counter to 0
                inner = 0;
                // Creating the corresponding directory
                dir_name = base_dir 
                            + "/" + std::to_string(m12) 
                            + "_" + std::to_string(g12)
                            + "/" + std::to_string(outer);
                fs::create_directories(dir_name);

                for ( double K : intra_Ks )
                {
                    // Updating the intra_K (inner coupling) counter
                    inner += 1;

                    dMatrix order1;
                    dMatrix order2;
                    dMatrix order;
                    for ( size_t iter = 0; iter < sim_num; ++iter)
                    {
                        // Setting up the initial conditions
                        params.initial_conditions = states[iter];

                        // Creating the corresponding directory
                        dir_name = base_dir 
                                    + "/" + std::to_string(m12) 
                                    + "_" + std::to_string(g12)
                                    + "/" + std::to_string(outer)
                                    + "/" + std::to_string(iter+1);

                        fs::create_directories(dir_name);

                        filename = std::to_string(outer)+"_"+std::to_string(inner);
                        ott_antonsen OA_params;
                        OA_params.mus         = mus;
                        OA_params.gammas      = gammas;
                        OA_params.connections = {{K, Q},
                                                 {Q, K}};
                        // std::cout << "Threads = " << std::thread::hardware_concurrency() << '\n';
                        // Setting up the `derivative` (RHS of the system of ODEs)
                        MyFunc OA = [&](const double time, const dVec& theta)
                        {
                            return ott_antonsen_basic(time, theta, OA_params);
                        };
                        params.derivative = OA;
                        // Solving the system of ODEs
                        SolverResults result_abm1 = adams_bashforth_moulton_solver(params);
                        // Calculating and organizing order parameters
                        auto [magnitude_dummy, order_sqr_dummy, order_dummy] = OA_order(result_abm1, num_modules);
                        // std::cout << "magnitude.size() = (" << magnitude_dummy.size() << "," << magnitude_dummy[0].size() << ")\n";
                        // Writing to file
                        write_matrix_to_file(magnitude_dummy, dir_name, filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

                        // Creating the data `dMatrix`s
                        order1.push_back(transpose(magnitude_dummy)[0]);
                        order2.push_back(transpose(magnitude_dummy)[1]);
                        order.push_back(transpose(magnitude_dummy)[2]);
                        // std::cout << "order1.size() = (" << order1.size() << "," << order1[0].size() << ")\n";
                        // std::cout << "order2.size() = (" << order2.size() << "," << order2[0].size() << ")\n";
                        // std::cout << "order.size() = (" << order.size() << "," << order[0].size() << ")\n";

                        // Updating the counter                        
                        counter += 1;
                        // Calculating the elapsed time (time consumed till this point)
                        auto end_time     = std::chrono::system_clock::now();
                        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time-start_time);
                        print_progress_bar_eta(counter,total_count,elapsed_time*(total_count-counter)/counter);
                    }
                    // std::cout << "order1.size() = (" << order1.size() << "," << order1[0].size() << ")\n";
                    // std::cout << "order2.size() = (" << order2.size() << "," << order2[0].size() << ")\n";
                    // std::cout << "order.size() = (" << order.size() << "," << order[0].size() << ")\n";
                    auto sim_stat_order1   = transpose(statistics(order1,1));
                    // std::cout << "sim_stat_order1.size() = (" << sim_stat_order1.size() << "," << sim_stat_order1[0].size()<< '\n';
                    auto sim_stat_order2   = transpose(statistics(order2,1));
                    // std::cout << "sim_stat_order2.size() = " << sim_stat_order2.size() << '\n';
                    auto sim_stat_order    = transpose(statistics(order,1));
                    // std::cout << "sim_stat_order.size() = " << sim_stat_order.size() << '\n';
                    auto time_stats_order1 = transpose(statistics_last_n_percent(sim_stat_order1[0], percentages));
                    // std::cout << "time_stats_order1.size() = " << time_stats_order1.size() << '\n';
                    auto time_stats_order2 = transpose(statistics_last_n_percent(sim_stat_order2[0], percentages));
                    // std::cout << "time_stats_order2.size() = " << time_stats_order2.size() << '\n';
                    auto time_stats_order  = transpose(statistics_last_n_percent(sim_stat_order[0], percentages));
                    // std::cout << "time_stats_order.size() = " << time_stats_order.size() << '\n';
                    for ( size_t ii = 0; ii < percentages.size(); ++ii)
                    {
                        order1_means[ii][inner-1][outer-1]   = time_stats_order1[0][ii];
                        order2_means[ii][inner-1][outer-1]   = time_stats_order2[0][ii];
                        order_means[ii][inner-1][outer-1]    = time_stats_order[0][ii];
                        order1_stddevs[ii][inner-1][outer-1] = time_stats_order1[1][ii];
                        order2_stddevs[ii][inner-1][outer-1] = time_stats_order2[1][ii];
                        order_stddevs[ii][inner-1][outer-1]  = time_stats_order[1][ii];
                        order1_cvs[ii][inner-1][outer-1]     = time_stats_order1[2][ii];
                        order2_cvs[ii][inner-1][outer-1]     = time_stats_order2[2][ii];
                        order_cvs[ii][inner-1][outer-1]      = time_stats_order[2][ii];
                    }
                    // std::cout << "K = " << K << '\n';
                }
                // std::cout << "Q = " << Q << '\n';
            }
            // Creating the corresponding directory
            dir_name = base_dir 
                        + "/" + std::to_string(m12) 
                        + "_" + std::to_string(g12);
            for ( size_t ii = 0; ii < percentages.size(); ++ii)
            {
                write_matrix_to_file(order1_means[ii], dir_name, "order1_means_"+std::to_string(ii), filetype, separator, comment, precision, format, alignment, stats_header, footer, append, col_width, skip_trailing_separator, binary);
                write_matrix_to_file(order2_means[ii], dir_name, "order2_means_"+std::to_string(ii), filetype, separator, comment, precision, format, alignment, stats_header, footer, append, col_width, skip_trailing_separator, binary);
                write_matrix_to_file(order_means[ii], dir_name, "order_means_"+std::to_string(ii), filetype, separator, comment, precision, format, alignment, stats_header, footer, append, col_width, skip_trailing_separator, binary);
                write_matrix_to_file(order1_stddevs[ii], dir_name, "order1_stddevss_"+std::to_string(ii), filetype, separator, comment, precision, format, alignment, stats_header, footer, append, col_width, skip_trailing_separator, binary);
                write_matrix_to_file(order2_stddevs[ii], dir_name, "order2_stddevs_"+std::to_string(ii), filetype, separator, comment, precision, format, alignment, stats_header, footer, append, col_width, skip_trailing_separator, binary);
                write_matrix_to_file(order_stddevs[ii], dir_name, "order_stddevs_"+std::to_string(ii), filetype, separator, comment, precision, format, alignment, stats_header, footer, append, col_width, skip_trailing_separator, binary);
                write_matrix_to_file(order1_cvs[ii], dir_name, "order1_cvs_"+std::to_string(ii), filetype, separator, comment, precision, format, alignment, stats_header, footer, append, col_width, skip_trailing_separator, binary);
                write_matrix_to_file(order2_cvs[ii], dir_name, "order2_cvs_"+std::to_string(ii), filetype, separator, comment, precision, format, alignment, stats_header, footer, append, col_width, skip_trailing_separator, binary);
                write_matrix_to_file(order_cvs[ii], dir_name, "order_cvs_"+std::to_string(ii), filetype, separator, comment, precision, format, alignment, stats_header, footer, append, col_width, skip_trailing_separator, binary);
            }
        }
    }

    // Setting up an output file stream
    std::ofstream out_file;
    out_file.open("two-modules-kq-done.txt", std::ios::trunc);
    // Calculating the elapsed time (time consumed till this point)
    auto finish_time = std::chrono::system_clock::now();
    auto total_time  = std::chrono::duration_cast<std::chrono::nanoseconds>(finish_time-start_time);
    out_file << "Finished in: " << total_time << '\n' << "Done!\n";
    // Closing the output file
    out_file.close();

    out_file.open("details.txt", std::ios::trunc);
    out_file << "Initial time = " << init_time << '\n';
    out_file << "Final time = " << final_time << '\n';
    out_file << "dt (constant) = " << dt << '\n';
    out_file << "num_modules = " << num_modules << '\n';
    out_file << "inter_K0 = " << inter_K0 << '\n';
    out_file << "intra_K0 = " << intra_K0 << '\n';
    out_file << "inter_dK = " << inter_dK << '\n';
    out_file << "intra_dK = " << intra_dK << '\n';
    out_file << "num_inter_K = " << num_inter_K << '\n';
    out_file << "num_intra_K = " << num_intra_K << '\n';
    out_file << "seed = " << seed << '\n';
    out_file << "sim_num = " << sim_num << '\n';
    out_file << "gammas: \n";
    for ( auto gamma : the_gammas )
    {
        out_file << gamma[0] << "\t" << gamma[1] << '\n';
    }
    out_file << "mus: \n";
    for ( auto mu : the_mus )
    {
        out_file << mu[0] << "\t" << mu[1] << '\n';
    }
    out_file << "percentages: \n";
    for ( auto p : percentages )
    {
        out_file << p << '\n';
    }
    out_file.close();

    out_file.open("two-modules-kq-done.txt", std::ios::app);
    out_file << "Total count = " << total_count << '\n';
    out_file << "Average time per simulation = " << total_time/total_count << '\n';
    out_file.close();
}

// > g++ -O3 -std=c++23 OA-kq.cpp -o OA-kq.exe
// > ./OA-kq.exe
