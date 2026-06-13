#include "../includes/initializers/initials.hpp"

int main() 
{
    size_t N = 8;
    unsigned seed = 42;
    std::cout << "random_uniform: ";
    dVec  v1 = random_uniform(N, 0.0, 1.0, seed);
    for (size_t i = 0; i < 3; ++i) std::cout << v1[i] << " ";
    std::cout << "...\n";

    std::cout << "random_normal: ";
    dVec  v2 = random_normal(N, 0.0, 1.0, seed);
    for (size_t i = 0; i < 3; ++i) std::cout << v2[i] << " ";
    std::cout << "...\n";

    std::cout << "random_circle: ";
    dVec  v3 = random_circle(N, seed);
    for (size_t i = 0; i < 3; ++i) std::cout << v3[i] << " ";
    std::cout << "...\n";

    std::cout << "splay: ";
    dVec  v4 = splay(N);
    for (size_t i = 0; i < 3; ++i) std::cout << v4[i] << " ";
    std::cout << "...\n";

    std::cout << "splay_perturbed: ";
    dVec  v5 = splay_perturbed(N, 0.2, seed);
    for (size_t i = 0; i < 3; ++i) std::cout << v5[i] << " ";
    std::cout << "...\n";

    std::cout << "module_by_condition (normal): ";
    dVec  v6 = module_by_condition(N, "normal", 0.0, 1.0, seed);
    for (size_t i = 0; i < 3; ++i) std::cout << v6[i] << " ";
    std::cout << "...\n";

    std::cout << "identical_modules (uniform): ";
    dVec  v7 = identical_modules(3, 2, "uniform", 0.0, 1.0, seed);
    for (size_t i = 0; i < 6; ++i) std::cout << v7[i] << " ";
    std::cout << "...\n";

    return 0;
} 