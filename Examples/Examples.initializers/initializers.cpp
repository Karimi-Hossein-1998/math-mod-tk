#include "../../includes/initializers/initials.hpp"

int main() 
{
    std::fstream file;
    file.open("initializers.txt", std::ios::out | std::ios::trunc);
    size_t N = 100;
    unsigned seed = 42;
    dVec  v1 = random_uniform(N, 0.0, 1.0, seed);
    file << "random_uniform:\n\n";
    file << "[ ";
    for (size_t i = 0; i < N-1; ++i) file << v1[i] << ", ";
    file << v1[N-1] << " ]\n";
    file << "\n";

    dVec  v2 = random_normal(N, 0.0, 1.0, seed);
    file << "random_normal:\n\n";
    file << "[ ";
    for (size_t i = 0; i < N-1; ++i) file << v2[i] << ", ";
    file << v2[N-1] << " ]\n";
    file << "\n";

    dVec  cauchy = random_cauchy(N, 0.2, 0.05, seed);
    file << "random_cauchy:\n\n";
    file << "[ ";
    for (size_t i = 0; i < N-1; ++i) file << cauchy[i] << ", ";
    file << cauchy[N-1] << " ]\n";
    file << "\n";

    dVec exponential = random_exponential(N, 0.1, seed);
    file << "random_exponential:\n\n";
    file << "[ ";
    for (size_t i = 0; i < N-1; ++i) file << exponential[i] << ", ";
    file << exponential[N-1] << " ]\n";
    file << "\n";

    dVec  v3 = random_circle(N, seed);
    file << "random_circle:\n\n";
    file << "[ ";
    for (size_t i = 0; i < N-1; ++i) file << v3[i] << ", ";
    file << v3[N-1] << " ]\n";
    file << "\n";

    dVec  v4 = splay(N);
    file << "splay:\n\n";
    file << "[ ";
    for (size_t i = 0; i < N-1; ++i) file << v4[i] << ", ";
    file << v4[N-1] << " ]\n";
    file << "\n";

    dVec  v5 = splay_perturbed(N, 0.01, seed);
    file << "splay_perturbed:\n\n";
    file << "[ ";
    for (size_t i = 0; i < N-1; ++i) file << v5[i] << ", ";
    file << v5[N-1] << " ]\n";
    file << "\n";

    dVec  v6 = module_by_condition(N, "normal", 0.0, 1.0, seed);
    file << "module_by_condition (normal):\n\n";
    file << "[ ";
    for (size_t i = 0; i < N-1; ++i) file << v6[i] << ", ";
    file << v6[N-1] << " ]\n";
    file << "\n";

    dVec  v7 = identical_modules(3, 2, "uniform", 0.0, 1.0, seed);
    file << "identical_modules (uniform):\n\n";
    file << "[ ";
    for (size_t i = 0; i < 5; ++i) file << v7[i] << ", ";
    file << v7[5] << " ]\n";
    file << "\n";

    file.close();
    return 0;
}

// > g++ -O3 -std=c++23 initializers.cpp -o initializers.exe
// > ./initializers.exe