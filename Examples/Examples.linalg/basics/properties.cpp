#include "../../../includes/linalg/basics/matrix-properties.hpp"
#include "../../../includes/utility/print.hpp"

int main()
{
    dMatrix A = {{1.0, 2.0, 3.0},
                {0.0, 5.0, 0.0},
                {7.0, 8.0, 9.0}};
    
    double trace_A = trace(A);
    std::cout << "The matrix A is:\n";
    print_matrix(A);

    std::cout << "And its trace is:\n";
    std::cout << trace_A << std::endl;

    double det_A = determinant(A);
    std::cout << "And its determinant is:\n";
    std::cout << det_A << std::endl;

    double minor22_A = minor(A, 2, 2);
    std::cout << "And its minor on 2nd row and 2nd column is:\n";
    std::cout << minor22_A << std::endl;

    dMatrix At = transpose(A);
    std::cout << "And its transpose is:\n";
    print_matrix(At);

    dMatrix cof_A = cofactor_matrix(A);
    std::cout << "And its cofactor is:\n";
    print_matrix(cof_A);

    dMatrix adjugate_A = adjugate(A);
    std::cout << "And its adjugate is:\n";
    print_matrix(adjugate_A);

    dMatrix inv_A = inverse(A);
    std::cout << "And its inverse is:\n";
    print_matrix(inv_A);

    bool A_symmetry = is_symmetric(A);
    std::cout << "And it is: ";
    (A_symmetry) ? std::cout << "SYMMETRIC\n" : std::cout << "NOT SYMMETRIC\n";

    bool A_identity = is_identity(A);
    std::cout << "And it is: ";
    (A_identity) ? std::cout << "IDENTITY\n" : std::cout << "NOT IDENTITY\n";

    bool A_diagonal = is_diagonal(A);
    std::cout << "And it is: ";
    (A_diagonal) ? std::cout << "DIAGONAL\n" : std::cout << "NOT DIAGONAL\n";

    bool A_uptri = is_upper_triangular(A);
    std::cout << "And it is: ";
    (A_uptri) ? std::cout << "UPPER TRIANGULAR\n" : std::cout << "NOT UPPER TRIANGULAR\n";

    bool A_lotri = is_lower_triangular(A);
    std::cout << "And it is: ";
    (A_lotri) ? std::cout << "LOWER TRIANGULAR\n" : std::cout << "NOT LOWER TRIANGULAR\n";

    double frob_norm_A = frobenius_norm(A);
    std::cout << "And its frobenius norm is:\n";
    std::cout << frob_norm_A << std::endl;

    double max_norm_A = max_norm(A);
    std::cout << "And its max/max-row-sum (infinity) norm is:\n";
    std::cout << max_norm_A << std::endl;

    double one_norm_A = one_norm(A);
    std::cout << "And its max-column-sum /1-norm is:\n";
    std::cout << one_norm_A << std::endl;

    double rank_A = rank(A);
    std::cout << "And its rank is:\n";
    std::cout << rank_A << std::endl;

}

// > g++ -O3 -std=c++23 properties.cpp -o properties.exe
// > ./properties.exe