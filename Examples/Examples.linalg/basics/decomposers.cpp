#include "../../../includes/linalg/basics/decomposers.hpp"
#include "../../../includes/utility/print.hpp"

int main()
{
    dMatrix A = {{1.0, -2.0, 0.0, -4.0},
                 {-5.0, 6.0, -7.0, 8.0},
                 {0.0, -8.0, 7.0, -6.0},
                 {-5.0, 4.0, -3.0, 0.0}};

    std::cout << "Matrix A:\n";
    print_matrix(A);
    auto [L,U,P] = lu_decomposition(A);

    std::cout << "L:\n";
    print_matrix(L);

    std::cout << "U:\n";
    print_matrix(U);

    std::cout << "Permutations:\n";
    print_vector(P);

    std::pair<dMatrix, dMatrix> qr = qr_decomposition(A);

    std::cout << "\n" << "Q:\n";
    print_matrix(qr.first);

    std::cout << "\n" << "R:\n";
    print_matrix(qr.second);
}

// > g++ -O3 -std=c++23 decomposers.cpp -o decomposers.exe
// > ./decomposers.exe