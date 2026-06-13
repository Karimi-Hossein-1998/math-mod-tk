#include "../includes/initializers/initials.hpp"
#include "../includes/utility/write.hpp"

int main() 
{
    // Output directory (current directory)
    std::string out_dir = ".";

    // Parameters for phase initialization
    size_t N = 30;
    unsigned seed = 123;
    std::string filetype = "tsv";
    std::string separator = "\t";
    std::string comment = "";
    std::string precision = "8";
    std::string format = "fixed";
    std::string alignment = "right";
    std::string header = "Phase initializations created from initials.hpp for testing purposes";
    std::string footer = "All set!";
    bool append = true;
    size_t col_width = 12;
    bool skip_trailing_separator = false;
    bool binary = false;

    // --- 1. Random uniform phases ---
    append = false;
    double min_val = -PI;
    double max_val = PI;
    auto phases_uniform = random_uniform(N, min_val, max_val, seed);
    comment = "Random Uniform Phases with Parameters: N = " + std::to_string(N) + 
              ", min_val = " + std::to_string(min_val) + 
              ", max_val = " + std::to_string(max_val) + 
              ", seed = " + std::to_string(seed);
    write_vector_to_file(phases_uniform, out_dir, "phases_uniform", filetype, separator, 
                        comment, precision, format, alignment, header, footer, append, 
                        col_width, skip_trailing_separator, binary);

    // --- 2. Random normal phases ---
    double mean = 0.0;
    double stddev = 1.0;
    auto phases_normal = random_normal(N, mean, stddev, seed);
    comment = "Random Normal Phases with Parameters: N = " + std::to_string(N) + 
              ", mean = " + std::to_string(mean) + 
              ", stddev = " + std::to_string(stddev) + 
              ", seed = " + std::to_string(seed);
    write_vector_to_file(phases_normal, out_dir, "phases_normal", filetype, separator, 
                        comment, precision, format, alignment, header, footer, append, 
                        col_width, skip_trailing_separator, binary);

    // --- 3. Random Cauchy phases ---
    double location = 0.0;
    double scale = 1.0;
    auto phases_cauchy = random_cauchy(N, location, scale, seed);
    comment = "Random Cauchy Phases with Parameters: N = " + std::to_string(N) + 
              ", location = " + std::to_string(location) + 
              ", scale = " + std::to_string(scale) + 
              ", seed = " + std::to_string(seed);
    write_vector_to_file(phases_cauchy, out_dir, "phases_cauchy", filetype, separator, 
                        comment, precision, format, alignment, header, footer, append, 
                        col_width, skip_trailing_separator, binary);

    // --- 4. Random exponential phases ---
    double lambda = 1.0;
    auto phases_exponential = random_exponential(N, lambda, seed);
    comment = "Random Exponential Phases with Parameters: N = " + std::to_string(N) + 
              ", lambda = " + std::to_string(lambda) + 
              ", seed = " + std::to_string(seed);
    write_vector_to_file(phases_exponential, out_dir, "phases_exponential", filetype, separator, 
                        comment, precision, format, alignment, header, footer, append, 
                        col_width, skip_trailing_separator, binary);

    // --- 5. Random circle phases ---
    auto phases_circle = random_circle(N, seed);
    comment = "Random Circle Phases with Parameters: N = " + std::to_string(N) + 
              ", seed = " + std::to_string(seed);
    write_vector_to_file(phases_circle, out_dir, "phases_circle", filetype, separator, 
                        comment, precision, format, alignment, header, footer, append, 
                        col_width, skip_trailing_separator, binary);

    // --- 6. Splay phases ---
    auto phases_splay = splay(N);
    comment = "Splay Phases with Parameters: N = " + std::to_string(N);
    write_vector_to_file(phases_splay, out_dir, "phases_splay", filetype, separator, 
                        comment, precision, format, alignment, header, footer, append, 
                        col_width, skip_trailing_separator, binary);

    // --- 7. Perturbed splay phases ---
    double amplitude = 0.1;
    auto phases_splay_perturbed = splay_perturbed(N, amplitude, seed);
    comment = "Perturbed Splay Phases with Parameters: N = " + std::to_string(N) + 
              ", amplitude = " + std::to_string(amplitude) + 
              ", seed = " + std::to_string(seed);
    write_vector_to_file(phases_splay_perturbed, out_dir, "phases_splay_perturbed", filetype, separator, 
                        comment, precision, format, alignment, header, footer, append, 
                        col_width, skip_trailing_separator, binary);

    // --- 8. Module by condition (uniform) ---
    size_t module_size = 10;
    std::string dist_type = "uniform";
    auto phases_module = module_by_condition(module_size, dist_type, min_val, max_val, seed);
    comment = "Module Phases (uniform) with Parameters: module_size = " + std::to_string(module_size) + 
              ", dist_type = " + dist_type + 
              ", min_val = " + std::to_string(min_val) + 
              ", max_val = " + std::to_string(max_val) + 
              ", seed = " + std::to_string(seed);
    write_vector_to_file(phases_module, out_dir, "phases_module", filetype, separator, 
                        comment, precision, format, alignment, header, footer, append, 
                        col_width, skip_trailing_separator, binary);

    // --- 9. Identical modules ---
    size_t num_modules = 3;
    auto phases_identical = identical_modules(module_size, num_modules, dist_type, min_val, max_val, seed);
    comment = "Identical Modules Phases with Parameters: module_size = " + std::to_string(module_size) + 
              ", num_modules = " + std::to_string(num_modules) + 
              ", dist_type = " + dist_type + 
              ", min_val = " + std::to_string(min_val) + 
              ", max_val = " + std::to_string(max_val) + 
              ", seed = " + std::to_string(seed);
    write_vector_to_file(phases_identical, out_dir, "phases_identical", filetype, separator, 
                        comment, precision, format, alignment, header, footer, append, 
                        col_width, skip_trailing_separator, binary);

    // --- Write all to a shared file (append mode) ---
    append = true;
    std::string phases_all_filename = "phases_all";
    header = "All phase initializations";
    
    // Write each set of phases to the combined file
    write_vector_to_file(phases_uniform, out_dir, phases_all_filename, filetype, separator, 
                        "Random uniform phases:", precision, format, alignment, header, footer, append, 
                        col_width, skip_trailing_separator, binary);
    write_vector_to_file(phases_normal, out_dir, phases_all_filename, filetype, separator, 
                        "Random normal phases:", precision, format, alignment, "", footer, append, 
                        col_width, skip_trailing_separator, binary);
    write_vector_to_file(phases_cauchy, out_dir, phases_all_filename, filetype, separator, 
                        "Random Cauchy phases:", precision, format, alignment, "", footer, append, 
                        col_width, skip_trailing_separator, binary);
    write_vector_to_file(phases_exponential, out_dir, phases_all_filename, filetype, separator, 
                        "Random exponential phases:", precision, format, alignment, "", footer, append, 
                        col_width, skip_trailing_separator, binary);
    write_vector_to_file(phases_circle, out_dir, phases_all_filename, filetype, separator, 
                        "Random circle phases:", precision, format, alignment, "", footer, append, 
                        col_width, skip_trailing_separator, binary);
    write_vector_to_file(phases_splay, out_dir, phases_all_filename, filetype, separator, 
                        "Splay phases:", precision, format, alignment, "", footer, append, 
                        col_width, skip_trailing_separator, binary);
    write_vector_to_file(phases_splay_perturbed, out_dir, phases_all_filename, filetype, separator, 
                        "Perturbed splay phases:", precision, format, alignment, "", footer, append, 
                        col_width, skip_trailing_separator, binary);
    write_vector_to_file(phases_module, out_dir, phases_all_filename, filetype, separator, 
                        "Module phases:", precision, format, alignment, "", footer, append, 
                        col_width, skip_trailing_separator, binary);
    write_vector_to_file(phases_identical, out_dir, phases_all_filename, filetype, separator, 
                        "Identical modules phases:", precision, format, alignment, "", footer, append, 
                        col_width, skip_trailing_separator, binary);

    std::cout << "Phase initializations written to files." << std::endl;
    return 0;
} 