#include "../../includes/network/topology.hpp"
// For write
#include "../../includes/utility/write.hpp"

int main()
{
    // Output directory (current directory)
    std::string out_dir = ".";

    // Parameters for adjacency matrices write function
    size_t                    N = 30;
    size_t                     M = 4;
    size_t                     K = 6;
    size_t                levels = 2;
    double                     p = 0.9;
    double                  beta = 0.6;
    double                weight = 1.0;
    double                  p_in = 0.9;
    double                 p_out = 0.1;
    double            min_weight = 0.1;
    double            max_weight = 1.0;
    double             weight_in = 1.0;
    double            weight_out = 0.5;
    double           level_decay = 0.5;
    unsigned                seed = 123;
    std::string         filename = "adj_all";
    std::string         filetype = "tsv";
    std::string        separator = "\t";
    std::string          comment = "";
    std::string        precision = "8";
    std::string           format = "fixed";
    std::string        alignment = "left";
    std::string           header = "adjacency matrices created from topology.hpp and test.cpp for testing purposes";
    std::string           footer = "All set!";
    bool                  append = true;
    size_t             col_width = 12;
    bool skip_trailing_separator = false;
    bool                  binary = false;

    append = false;

    // --- 1.1. Random adjacency ---
    auto adj_random = random(N, min_weight, max_weight, seed);
    comment = "Random Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", min_weight = " + std::to_string(min_weight) + ", max_weight = " + std::to_string(max_weight) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_random, out_dir, "adj_random", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    // --- 1.2. Random symmetric adjacency ---
    auto adj_random_sym = random_symmetric(N, min_weight, max_weight, seed);
    comment = "Random Symmetric Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", min_weight = " + std::to_string(min_weight) + ", max_weight = " + std::to_string(max_weight) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_random_sym, out_dir, "adj_random_symmetric", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    // --- 2.1. Erdos-Renyi adjacency ---
    auto adj_er = erdos_renyi(N, p, min_weight, max_weight, seed);
    comment = "Erdos-Renyi Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", p = " + std::to_string(p) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_er, out_dir, "adj_erdosrenyi", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    // --- 2.2. Erdos-Renyi uniform (N*(N-1)*p edges) adjacency ---
    auto adj_er_uni = erdos_renyi_uniform(N, p, min_weight, max_weight, seed);
    comment = "Erdos-Renyi Uniform (N*(N-1)*p edges) Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", p = " + std::to_string(p) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_er_uni, out_dir, "adj_erdosrenyi_uniform", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    // --- 2.3. Erdos-Renyi symmetric adjacency ---
    auto adj_er_sym = erdos_renyi_symmetric(N, p, min_weight, max_weight, seed);
    comment = "Erdos-Renyi Symmetric Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", p = " + std::to_string(p) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_er_sym, out_dir, "adj_erdosrenyi_symmetric", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    // --- 2.4. Erdos-Renyi symmetric uniform (N*(N-1)*p edges) adjacency ---
    auto adj_er_sym_uni = erdos_renyi_symmetric_uniform(N, p, min_weight, max_weight, seed);
    comment = "Erdos-Renyi Symmetric Uniform (N*(N-1)*p edges) Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", p = " + std::to_string(p) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_er_sym_uni, out_dir, "adj_erdosrenyi_symmetric_uniform", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    
    // --- 3.1. Small-world adjacency ---
    auto adj_sw = small_world(N, K, beta, weight, seed);
    comment = "Small-world Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", K = " + std::to_string(K) + ", beta = " + std::to_string(beta) + ", weight = " + std::to_string(weight) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_sw, out_dir, "adj_smallworld", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    // --- 3.2. Small-world directed adjacency ---
    auto adj_sw_dir = small_world_directed(N, K, beta, weight, seed);
    comment = "Small-world Directed Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", K = " + std::to_string(K) + ", beta = " + std::to_string(beta) + ", weight = " + std::to_string(weight) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_sw_dir, out_dir, "adj_smallworld_directed", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    // --- 4. Modular adjacency ---
    auto adj_mod = modular(N, M, p_in, p_out, weight_in, weight_out, seed);
    comment = "Modular Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", M = " + std::to_string(M) + ", p_in = " + std::to_string(p_in) + ", p_out = " + std::to_string(p_out) + ", weight_in = " + std::to_string(weight_in) + ", weight_out = " + std::to_string(weight_out) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_mod, out_dir, "adj_modular", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    // --- 5. Hierarchical adjacency ---
    auto adj_hier = hierarchical(N, levels, p_in, p_out, weight_in, weight_out, level_decay, seed, M);
    comment = "Hierarchical Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", levels = " + std::to_string(levels) + ", p_in = " + std::to_string(p_in) + ", p_out = " + std::to_string(p_out) + ", weight_in = " + std::to_string(weight_in) + ", weight_out = " + std::to_string(weight_out) + ", seed = " + std::to_string(seed) + ", base_module_num = " + std::to_string(M);
    write_matrix_to_file(adj_hier, out_dir, "adj_hierarchical", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    Vec<dMatrix> layers = {adj_er, adj_er_uni, adj_er_sym, adj_er_sym_uni, adj_mod};
    // --- 6. Multilayered adjacency ---
    auto adj_multi_layer = multilayered(layers);
    comment = "Multi-layered Adjacency dMatrix with matrices: adj_er, adj_er_uni, adj_er_sym, adj_er_sym_uni, adj_mod.";
    write_matrix_to_file(adj_multi_layer, out_dir, "adj_multi_layer", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    Vec<dMatrix> layers_er = {adj_er, adj_er_uni, adj_er_sym, adj_er_sym_uni};
    dVec          weights = {0.4, 0.05, 0.3, 0.25};
    // --- 7. Effective Multiplex reduced adjacency ---
    auto adj_eff_er = effective_multiplex(layers_er, weights);
    comment = "Effective Multiplex Adjacency dMatrix with matrices: adj_er, adj_er_uni, adj_er_sym, adj_er_sym_uni.\nThe wights correspondingly are : \n[ 0.4, 0.05, 0.3, 0.25 ].";
    write_matrix_to_file(adj_eff_er, out_dir, "adj_eff_er", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    // --- Write all to a shared file (append mode) ---
    std::string adj_all_filename = "adj_all";
    header = "All adjacency matrices";

    comment = " Parameters: N = " + std::to_string(N) + ", min_weight = " + std::to_string(min_weight) + ", max_weight = " + std::to_string(max_weight) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_random, out_dir, adj_all_filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    append = true;

    comment = " Parameters: N = " + std::to_string(N) + ", p = " + std::to_string(p) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_er, out_dir, adj_all_filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    comment = " Small-world Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", K = " + std::to_string(K) + ", beta = " + std::to_string(beta) + ", weight = " + std::to_string(weight) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_sw, out_dir, adj_all_filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    comment = " Parameters: N = " + std::to_string(N) + ", M = " + std::to_string(M) + ", p_in = " + std::to_string(p_in) + ", p_out = " + std::to_string(p_out) + ", weight_in = " + std::to_string(weight_in) + ", weight_out = " + std::to_string(weight_out) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_mod, out_dir, adj_all_filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    comment = " Parameters: N = " + std::to_string(N) + ", levels = " + std::to_string(levels) + ", p_in = " + std::to_string(p_in) + ", p_out = " + std::to_string(p_out) + ", weight_in = " + std::to_string(weight_in) + ", weight_out = " + std::to_string(weight_out) + ", seed = " + std::to_string(seed) + ", base_module_num = " + std::to_string(M);
    write_matrix_to_file(adj_hier, out_dir, adj_all_filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    std::cout << "Adjacency matrices written to files." << std::endl;

    return 0;
}

// > g++ -O3 -std=c++23 adjacency.cpp -o adjacency.exe
// > ./adjacency.exe