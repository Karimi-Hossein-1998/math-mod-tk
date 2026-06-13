#include "../includes/network/topology.hpp"
#include "../includes/utility/write.hpp"

int main() {
    // Output directory (current directory)
    std::string out_dir = ".";

    // Parameters for adjacency matrices
    size_t N = 30;
    size_t M = 4;
    size_t K = 6;
    size_t levels = 2;
    double p = 0.9;
    double beta = 0.6;
    double weight = 1.0;
    double p_in = 0.9;
    double p_out = 0.1;
    double min_weight = 0.1;
    double max_weight = 1.0;
    double weight_in = 1.0;
    double weight_out = 0.5;
    double level_decay = 0.5;
    unsigned seed = 123;
    std::string filename = "adj_all";
    std::string filetype = "tsv";
    std::string separator = "\t";
    std::string comment = "";
    std::string precision = "8";
    std::string format = "fixed";
    std::string alignment = "left";
    std::string header = "adjacency matrices created from topology.hpp and test.cpp for testing purposes";
    std::string footer = "All set!";
    bool append = true;
    size_t col_width = 12;
    bool skip_trailing_separator = false;
    bool binary = false;

    // --- 1. Random adjacency ---
    append = false;
    auto adj_random = random(N, min_weight, max_weight, seed);
    comment = "Random Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", min_weight = " + std::to_string(min_weight) + ", max_weight = " + std::to_string(max_weight) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_random, out_dir, "adj_random", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    // --- 2. Erdos-Renyi adjacency ---
    auto adj_er = erdos_renyi(N, p, min_weight, max_weight, seed);
    comment = "Erdos-Renyi Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", p = " + std::to_string(p) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_er, out_dir, "adj_erdosrenyi", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    // --- 3. Small-world adjacency ---
    auto adj_sw = small_world(N, K, beta, weight, seed);
    comment = "Small-world Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", K = " + std::to_string(K) + ", beta = " + std::to_string(beta) + ", weight = " + std::to_string(weight) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_sw, out_dir, "adj_smallworld", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    // --- 4. Modular adjacency ---
    auto adj_mod = modular(N, M, p_in, p_out, weight_in, weight_out, seed);
    comment = "Modular Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", M = " + std::to_string(M) + ", p_in = " + std::to_string(p_in) + ", p_out = " + std::to_string(p_out) + ", weight_in = " + std::to_string(weight_in) + ", weight_out = " + std::to_string(weight_out) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_mod, out_dir, "adj_modular", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
    // --- 5. Hierarchical adjacency ---
    auto adj_hier = hierarchical(N, levels, p_in, p_out, weight_in, weight_out, level_decay, seed, M);
    comment = "Hierarchical Adjacency dMatrix with Parameters: N = " + std::to_string(N) + ", levels = " + std::to_string(levels) + ", p_in = " + std::to_string(p_in) + ", p_out = " + std::to_string(p_out) + ", weight_in = " + std::to_string(weight_in) + ", weight_out = " + std::to_string(weight_out) + ", seed = " + std::to_string(seed) + ", base_module_num = " + std::to_string(M);
    write_matrix_to_file(adj_hier, out_dir, "adj_hierarchical", filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);

    // --- Write all to a shared file (append mode) ---
    append = true;
    std::string adj_all_filename = "adj_all";
    header = "All adjacency matrices";
    comment = " Parameters: N = " + std::to_string(N) + ", min_weight = " + std::to_string(min_weight) + ", max_weight = " + std::to_string(max_weight) + ", seed = " + std::to_string(seed);
    write_matrix_to_file(adj_random, out_dir, adj_all_filename, filetype, separator, comment, precision, format, alignment, header, footer, append, col_width, skip_trailing_separator, binary);
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