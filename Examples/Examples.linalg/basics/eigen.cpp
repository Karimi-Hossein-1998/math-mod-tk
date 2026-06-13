#include "../../../includes/linalg/basics/eigen.hpp"
#include "../../../includes/utility/print.hpp"

int main()
{
    dMatrix A1 = {{1.0, 2.0, 3.0},
                {0.0, 5.0, 0.0},
                {7.0, 8.0, 9.0}};
    
    std::cout << "The matrix A1 is:\n";
    print_matrix(A1);

    dMatrix A2 = {{2.0, 1.0, 0.0, 0.0},
                {2.0, 3.0, 0.0, 0.0},
                {0.0, 0.0, 4.0, 0.0},
                {0.0, 0.0, 0.0, 1.0}};
    
    std::cout << "The matrix A2 is:\n";
    print_matrix(A2);

    std::pair<dVec, dMatrix> eigens = eigen_qr(A2, 200,1.0e-4);
    std::cout << "Eigenvalues:\n";
    print_vector(eigens.first);
    std::cout << "Eigenvectors:\n";
    print_matrix(eigens.second);

    std::pair<double,dVec> pow_eigen_A2 = power_method(A2, 200, 1.0e-4);
    std::cout << "Dominant eigenvalue of A2 is:\n";
    std::cout << pow_eigen_A2.first << std::endl;
    std::cout << "Its corresponding eigenvector is:\n";
    print_vector(pow_eigen_A2.second);

    std::pair<double,dVec> pow_eigen_A1 = power_method(A1, 200, 1.0e-4);
    std::cout << "Dominant eigenvalue of A1 is:\n";
    std::cout << pow_eigen_A1.first << std::endl;
    std::cout << "Its corresponding eigenvector is:\n";
    print_vector(pow_eigen_A1.second);
}

// > g++ -O3 -std=c++23 eigen.cpp -o eigen.exe
// > ./eigen.exe