#include "../includes/typedefs/header.hpp"
#include "../includes/solvers/solver.hpp"
#include "../includes/initializers/initials.hpp"
#include "../includes/models/ott-antonsen.dim-red/oa-basic.hpp"
#include "../includes/models/ott-antonsen.dim-red/OA-expanded.hpp"
#include "../includes/models/ott-antonsen.dim-red/OA-helper.hpp"
#include "../includes/utility/print.hpp"
#include "../includes/utility/write.hpp"
#include <chrono>
#include <filesystem>
#include <cstdlib>
#include <thread>

namespace fs = std::filesystem;

// Parameter bundle configuring a modular OA run (flexible, ensemble-aware)
struct OAModularRunParams
{
    // base community block size (number of lowest-level modules per group)
    size_t num_modules = 2;
    // hierarchy levels (H >= 1). Total number of base communities C = num_modules * 2^{H-1}
    size_t hierarchy_level = 3;

    // distributions for community intrinsic parameters (rows of pairs)
    dMatrix mus    = {{-0.2,-0.2},{-0.2,0.2},{0.0,0.0},{0.2,0.2}};
    dMatrix gammas = {{0.05,0.05},{0.05,0.10},{0.10,0.10}};

    // coupling (intra/inter at base scale)
    double inter_K0 = 0.001;
    double intra_K0 = 0.001;
    double inter_dK = 0.004;
    double intra_dK = 0.004;
    size_t num_inter_K = 500;
    size_t num_intra_K = 500;

    // ensembles / seeds
    size_t simulations = 21;
    wVec seeds;

    // solver params
    SolverParameters solver_params;

    // filesystem
    std::string base_dir = "run_oa";

    // threads
    size_t num_threads = 3;

    // I/O precision
    std::string file_precision = "11";
    std::string file_format = "fixed";

    OAModularRunParams()
    {
        for ( size_t i = 0; i < simulations; ++i ) seeds.push_back(i*1234);
        solver_params.dt = 1.0 / 16.0;
        solver_params.t0 = 0.0;
        solver_params.t1 = 200.0;
    }
};

// Helper: convert Vec<Vec<complex>> (layers) into dMatrix of doubles suitable for write_matrix_to_file
// Each group in a layer becomes three columns: Re, Im, Magnitude
static dMatrix layers_complex_to_matrix(const Vec<Vec<std::complex<double>>>& layers)
{
    dMatrix out;
    out.reserve(layers.size());
    for (const auto& layer : layers)
    {
        dVec row;
        row.reserve(layer.size()*3);
        for (const auto& c : layer)
        {
            row.push_back(c.real());
            row.push_back(c.imag());
            row.push_back(std::abs(c));
        }
        out.push_back(row);
    }
    return out;
}

// Flexible OA run matching the structure/aesthetics of special_modular_run_flex.cpp
void run_special_modular_oa(const OAModularRunParams& params_in)
{
    auto params = params_in; // local modifiable copy

    std::cout << "Initializing OA flexible simulations...\n";

    const size_t num_modules = params.num_modules;
    const size_t H = params.hierarchy_level;
    const size_t simulations = params.simulations;
    const size_t num_threads = params.num_threads;

    // compute base number of communities
    const size_t C = num_modules * (1u << (H - 1));

    // build inter / intra K series
    dVec inter_Ks; inter_Ks.reserve(params.num_inter_K);
    for ( size_t i = 0; i < params.num_inter_K; ++i ) inter_Ks.push_back(params.inter_K0 + i*params.inter_dK);
    dVec intra_Ks;
    if (params.num_intra_K <= 1) intra_Ks.push_back(params.intra_K0);
    else { intra_Ks.reserve(params.num_intra_K); for (size_t i=0;i<params.num_intra_K;++i) intra_Ks.push_back(params.intra_K0 + i*params.intra_dK); }

    // prepare seeds
    wVec seeds = params.seeds;
    if (seeds.size() < simulations) { seeds.clear(); for (size_t i=0;i<simulations;++i) seeds.push_back(i*1234); }

    // header for output files: for each group we export Re,Im,Mag
    std::string header = "";
    for ( size_t i = 0; i < C; ++i ) header += "Re\tIm\tMag\t";
    header += "Time";

    // prepare directories
    fs::create_directories(params.base_dir);
    std::ofstream dirs((params.base_dir + "/directories.txt"), std::ios::out | std::ios::trunc);

    // Enumerate parameter blocks and create directories + directory listing. The outer-most
    // parallel worker will iterate the same parameter space per-thread (partitioned by seed).
    size_t blocks_total = intra_Ks.size() * params.mus.size() * params.gammas.size();
    size_t current = 0;
    auto start_time = std::chrono::system_clock::now();
    for ( auto intra_K : intra_Ks )
    {
        std::string intra_dir = params.base_dir + "/intra_K_" + std::to_string(intra_K);
        std::replace(intra_dir.begin(), intra_dir.end(), '.', '_');
        fs::create_directories(intra_dir);

        for ( size_t mu_idx = 0; mu_idx < params.mus.size(); ++mu_idx )
        {
            const auto& mu_pair = params.mus[mu_idx];
            for ( size_t gamma_idx = 0; gamma_idx < params.gammas.size(); ++gamma_idx )
            {
                const auto& gamma_pair = params.gammas[gamma_idx];
                double mu1 = mu_pair[0];
                double mu2 = mu_pair[1];
                double gamma1 = gamma_pair[0];
                double gamma2 = gamma_pair[1];

                std::string dir_name = intra_dir + "/mu1_" + std::to_string(mu1) + "_mu2_" + std::to_string(mu2) +
                                       "_gamma1_" + std::to_string(gamma1) + "_gamma2_" + std::to_string(gamma2);
                std::replace(dir_name.begin(), dir_name.end(), '.', '_');
                dirs << dir_name << '\n';
                fs::create_directories(dir_name);

                ++current;
                auto now = std::chrono::system_clock::now();
                auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(now - start_time);
                auto eta = elapsed * ( blocks_total - current ) / current;
                print_progress_bar_eta(current, blocks_total, eta, 48);
            }
        }
    }

    dirs.close();

    // For each (intra_K, mu, gamma) parameter block, pre-generate initial states and
    // then parallelise across simulations (threads created per-block as in the
    // Kuramoto flexible runner). This mirrors `special_modular_run_flex.cpp`.
    std::atomic_size_t completed{0};
    size_t total_jobs = simulations * intra_Ks.size() * params.mus.size() * params.gammas.size() * inter_Ks.size();

    for ( auto intra_K : intra_Ks )
    {
        std::string intra_dir = params.base_dir + "/intra_K_" + std::to_string(intra_K);
        std::replace(intra_dir.begin(), intra_dir.end(), '.', '_');

        for ( size_t mu_idx = 0; mu_idx < params.mus.size(); ++mu_idx )
        {
            const auto& mu_pair = params.mus[mu_idx];
            for ( size_t gamma_idx = 0; gamma_idx < params.gammas.size(); ++gamma_idx )
            {
                const auto& gamma_pair = params.gammas[gamma_idx];
                double mu1 = mu_pair[0];
                double mu2 = mu_pair[1];
                double gamma1 = gamma_pair[0];
                double gamma2 = gamma_pair[1];

                std::string dir_name = intra_dir + "/mu1_" + std::to_string(mu1) + "_mu2_" + std::to_string(mu2) +
                                       "_gamma1_" + std::to_string(gamma1) + "_gamma2_" + std::to_string(gamma2);
                std::replace(dir_name.begin(), dir_name.end(), '.', '_');

                // Build OA parameter vectors for C base communities by tiling the two-component specs
                dVec gammas(C, gamma1);
                dVec mus(C, mu1);
                for ( size_t b = 0; b < C; ++b ) if ((b % 2) == 1) { gammas[b] = gamma2; mus[b] = mu2; }

                // pre-generate initial conditions for each ensemble (per-block)
                dMatrix init_states(simulations, dVec(2*C, 0.0));
                for ( size_t s = 0; s < simulations; ++s )
                {
                    std::mt19937_64 rng(seeds[s]);
                    std::uniform_real_distribution<double> urho(0.01, 0.2);
                    std::uniform_real_distribution<double> uphi(-PI, PI);
                    for ( size_t c = 0; c < C; ++c )
                    {
                        double rho = urho(rng);
                        double phi = uphi(rng);
                        init_states[s][2*c + 0] = rho * std::cos(phi);
                        init_states[s][2*c + 1] = rho * std::sin(phi);
                    }
                }

                // parallel loop over simulations for this parameter block
                size_t chunk_size = std::max<size_t>(1, simulations / num_threads);
                Vec<std::jthread> threads;
                for ( size_t thread_idx = 0; thread_idx < num_threads; ++thread_idx )
                {
                    threads.emplace_back([&,thread_idx]()
                    {
                        size_t start_idx = thread_idx * chunk_size;
                        size_t end_idx   = (thread_idx == num_threads - 1) ? simulations : (thread_idx + 1) * chunk_size;
                        for ( size_t s = start_idx; s < end_idx; ++s )
                        {
                            size_t j = 0;
                            for ( size_t inter_j = 0; inter_j < inter_Ks.size(); ++inter_j )
                            {
                                double inter_K = inter_Ks[inter_j];
                                // build coupling matrix
                                dMatrix couplings(C, dVec(C, intra_K));
                                for ( size_t p = 0; p < C; ++p )
                                    for ( size_t q = 0; q < C; ++q )
                                        couplings[p][q] = (p == q) ? intra_K : inter_K;

                                ott_antonsen oa_params{ gammas, mus, couplings };

                                auto local_sp = params.solver_params;
                                local_sp.initial_conditions = init_states[s];
                                local_sp.derivative = [&](double t, const dVec& y){ return ott_antonsen_basic(t, y, oa_params); };

                                auto results = adams_bashforth_solver(local_sp);

                                // write time-series for this run
                                for (size_t ti = 0; ti < results.solution.size(); ++ti)
                                {
                                    const dVec& state = results.solution[ti];
                                    auto base_pairs = state_to_rhophi_from_reim(state);
                                    auto layers_cplx = build_hierarchical_layers_complex(base_pairs, num_modules, H, dVec(C, 1.0 / static_cast<double>(C)));
                                    auto out_matrix = layers_complex_to_matrix(layers_cplx);
                                    write_matrix_to_file(
                                        out_matrix,
                                        dir_name,
                                        "state_" + std::to_string(s) + "_" + std::to_string(inter_j) + "_" + std::to_string(j),
                                        "tsv",
                                        "\t",
                                        "",
                                        params.file_precision,
                                        params.file_format,
                                        "",
                                        std::to_string(results.time_points[ti]),
                                        "",
                                        true
                                    );
                                }

                                // final state
                                const dVec& final_state = results.solution.back();
                                auto base_pairs = state_to_rhophi_from_reim(final_state);
                                auto layers_cplx = build_hierarchical_layers_complex(base_pairs, num_modules, H, dVec(C, 1.0 / static_cast<double>(C)));
                                auto out_matrix = layers_complex_to_matrix(layers_cplx);

                                write_matrix_to_file(
                                    out_matrix,
                                    dir_name,
                                    "final_order_parameter_" + std::to_string(s) + "_" + std::to_string(inter_j),
                                    "tsv",
                                    "\t",
                                    "",
                                    params.file_precision,
                                    params.file_format,
                                    "",
                                    header
                                );

                                ++j;
                                // size_t done = completed.fetch_add(1) + 1;
                                // if ((done % 100) == 0 || done == total_jobs)
                                // {
                                //     auto now2 = std::chrono::system_clock::now();
                                //     auto elapsed2 = std::chrono::duration_cast<std::chrono::nanoseconds>(now2 - start_time);
                                //     auto eta2 = elapsed2 * (total_jobs - done) / std::max<size_t>(1, done);
                                //     print_progress_bar_eta(done, total_jobs, eta2, 48);
                                // }
                            }
                        }
                    });
                }
                // threads join on scope exit (jthreads destructor)
            }
        }
    }

    auto end_time = std::chrono::system_clock::now();
    auto total_elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time).count();
    std::cout << "Total time (ns): " << total_elapsed << "\n";
}

int main()
{
    OAModularRunParams params;
    params.num_inter_K = 500;
    params.num_intra_K = 500;
    params.num_modules = 2;
    params.hierarchy_level = 1;
    // params.num_threads = std::max<size_t>(1, std::thread::hardware_concurrency() - 1);
    params.num_threads = 7;
    params.solver_params.dt = 1./16.;
    params.solver_params.t0 = 0.0;
    params.solver_params.t1 = 200.0;


    run_special_modular_oa(params);
    return 0;
}
