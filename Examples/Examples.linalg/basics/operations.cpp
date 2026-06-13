#include "../../../includes/linalg/basics/matrix-operations.hpp"
#include "../../../includes/utility/print.hpp"

int main()
{
    dMatrix A1 = {{1.0, 2.0, 3.0},
                {0.0, 5.0, 0.0},
                {7.0, 8.0, 9.0}};
    
    std::cout << "The matrix A1 is:\n";
    print_matrix(A1);

    dMatrix A2 = {{0.0, 2.0, 3.0},
                {4.0, 5.0, 6.0},
                {7.0, 0.0, 9.0}};
    
    std::cout << "The matrix A2 is:\n";
    print_matrix(A2);

    dMatrix A3 = {{0.0, 2.0, 3.0, 4.0},
                {5.0, 6.0, 7.0, 0.0},
                {9.0, 0.0, 11.0, 12.0}};
    
    std::cout << "The matrix A3 is:\n";
    print_matrix(A3);

    dMatrix A4 = {{0.0, 2.0, 3.0},
                {4.0, 5.0, 6.0},
                {7.0, 0.0, 9.0},
                {0.0, 11.0, 12.0}};
    
    std::cout << "The matrix A4 is:\n";
    print_matrix(A4);

    dMatrix A5 = {{0.0, 2.0, 3.0, 4.0},
                {5.0, 6.0, 7.0, 0.0},
                {9.0, 0.0, 11.0, 12.0},
                {0.0, 0.0, 15.0, 16.0},
                {17.0, 0.0, 19.0, 20.0}};
    
    std::cout << "The matrix A5 is:\n";
    print_matrix(A5);

    
    dVec v1 = {10.0, 11.0, 12.0};
    triple v1t = {10.0, 11.0, 12.0};    
    std::cout << "The vector v1 is:\n";
    print_vector(v1);

    dVec v2 = {13.0, 14.0, 15.0, 16.0, 17.0};

    std::cout << "The vector v2 is:\n";
    print_vector(v2);

    dVec v3 = {-1.0, 5.75, -0.25};
    triple v3t = {-1.0, 5.75, -0.25};
    std::cout << "The vector v3 is:\n";
    print_vector(v3);

    double dot_v1_v3 = dot_product(v1, v3);
    std::cout << "v1 . v3 = " << dot_v1_v3 << '\n';

    triple cross_v1_v3  = cross_product(v1t,v3t);
    std::cout << "v1 x v3 = [ " << cross_v1_v3[0] << ", " << cross_v1_v3[1] << ", " << cross_v1_v3[2] << " ]\n";

    triple cross_v3_v1  = cross_product(v3t,v1t);
    std::cout << "v3 x v1 = [ " << cross_v3_v1[0] << ", " << cross_v3_v1[1] << ", " << cross_v3_v1[2] << " ]\n";

    triple cross_v3_v3  = cross_product(v3t,v3t);
    std::cout << "v3 x v3 = [ " << cross_v3_v3[0] << ", " << cross_v3_v3[1] << ", " << cross_v3_v3[2] << " ]\n";

    dMatrix mm_A1_A2 = matrix_multiply(A1, A2);
    std::cout << "A1 multiplied by A2 is:\n";
    print_matrix(mm_A1_A2);

    dMatrix mm_A5_A4 = matrix_multiply(A5, A4);
    std::cout << "A5 multiplied by A4 is:\n";
    print_matrix(mm_A5_A4);

    dVec mm_A1_v1 = matrix_multiply(A1, v1);
    std::cout << "A1 multiplied by v1 is:\n";
    print_vector(mm_A1_v1);

    dMatrix elwm_A1_A2 = elementwise_multiply(A1,A2);
    std::cout << "A1 multiplied by A2 (elementwise) is:\n";
    print_matrix(elwm_A1_A2);

    std::cout << "NOTE: You can do the same for two 1D-vectors!\n\n";

    dMatrix add_A1_A2 = add(A1,A2);
    std::cout << "A1 + A2 is:\n";
    print_matrix(add_A1_A2);

    std::cout << "NOTE: You can do the same for two 1D-vectors!\n\n";

    dMatrix sub_A1_A2 = subtract(A1,A2);
    std::cout << "A1 - A2 is:\n";
    print_matrix(sub_A1_A2);

    std::cout << "NOTE: You can do the same for two 1D-vectors!\n\n";

    dMatrix sm_2_A3 = scalar_multiply(2.0, A3);
    std::cout << "2.0 * A3 is:\n";
    print_matrix(sm_2_A3);

    std::cout << "NOTE: You can do the same for two 1D-vectors!\n\n";

    dVec vectm_v2_A5 = vector_transpose_multiply(v2,A5);
    std::cout << "v2 . A5 is:\n";
    print_vector(vectm_v2_A5);

    dMatrix outer_v1_v2 = outer_product(v1,v2);
    std::cout << "v1 ⊗ v2 is:\n";
    print_matrix(outer_v1_v2);

    dMatrix outer_A1_A5 = matrix_outer_product(A1,A5);
    std::cout << "A1 ⊗ A5 is:\n";
    print_matrix(outer_A1_A5);

    dMatrix transpose_v1 = vector_transpose(v1);
    std::cout << "v1 transpose (v1 in the row form) is:\n";
    print_matrix(transpose_v1);

    dVec v1_4D = {1.0, 2.0, 3.0, 4.0};
    std::cout << "v1_4D is:\n";
    print_vector(v1_4D);
    dVec v2_4D = {-1.0, 2.0, -3.0, 4.0};
    std::cout << "v2_4D is:\n";
    print_vector(v2_4D);
    dVec v3_4D = {-4.0, 3.0, -2.0, 1.0};
    std::cout << "v3_4D is:\n";
    print_vector(v3_4D);
    dMatrix vectors = {v1_4D, v2_4D, v3_4D};
    dVec gcp = generalized_cross_product(vectors);
    std::cout << "generalized cross product of v1_4D, v2_4D, and v3_4D is:\n";
    print_vector(gcp);
}

// > g++ -O3 -std=c++23 operations.cpp -o operations.exe
// > ./operations.exe