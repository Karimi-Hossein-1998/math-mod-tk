# Linear Algebra Basics (`basics/`)

This directory contains headers for fundamental linear algebra operations on matrices and vectors. These functions provide essential computational tools for matrix manipulation, decomposition, and analysis.

## Table of Contents

1. [Linear Algebra Basics (`basics/`)](#linear-algebra-basics-basics)
    1. [Table of Contents](#table-of-contents)
    2. [Overview](#overview)
    3. [1. dMatrix Operations (`matrix-operations.hpp`)](#1-dmatrix-operations-matrix-operationshpp)
        1. [Dot Product](#dot-product)
        2. [Cross Product](#cross-product)
        3. [dMatrix-Vector Multiplication](#dmatrix-vector-multiplication)
        4. [dMatrix-dMatrix Multiplication](#dmatrix-dmatrix-multiplication)
        5. [Element-wise Multiplication](#element-wise-multiplication)
        6. [Vector and dMatrix Addition](#vector-and-dmatrix-addition)
        7. [Vector and dMatrix Subtraction](#vector-and-dmatrix-subtraction)
        8. [Scalar Multiplication](#scalar-multiplication)
        9. [Vector Norm](#vector-norm)
    4. [2. dMatrix Properties (`matrix-properties.hpp`)](#2-dmatrix-properties-matrix-propertieshpp)
        1. [Trace](#trace)
        2. [Determinant](#determinant)
        3. [Minor](#minor)
        4. [Transpose](#transpose)
        5. [Cofactor dMatrix](#cofactor-dmatrix)
        6. [Adjugate (Classical Adjoint)](#adjugate-classical-adjoint)
        7. [Inverse](#inverse)
        8. [Rank](#rank)
        9. [Condition Number](#condition-number)
    5. [3. dMatrix Decompositions (`decomposers.hpp`)](#3-dmatrix-decompositions-decomposershpp)
        1. [LU Decomposition](#lu-decomposition)
        2. [QR Decomposition](#qr-decomposition)
        3. [Cholesky Decomposition](#cholesky-decomposition)
        4. [LDLT Decomposition](#ldlt-decomposition)
        5. [SVD Decomposition](#svd-decomposition)
    6. [4. Eigenvalue Computation (`eigen.hpp`)](#4-eigenvalue-computation-eigenhpp)
        1. [Power Method](#power-method)
        2. [Hessenberg Reduction](#hessenberg-reduction)
        3. [QR Eigenvalue Algorithm](#qr-eigenvalue-algorithm)
    7. [Interdependencies](#interdependencies)

## Overview

The basics module is organized into the following components:

- **`matrix-operations.hpp`** - Vector and matrix operations (products, sums, scaling)
- **`matrix-properties.hpp`** - dMatrix properties and transformations (trace, determinant, inverse, transpose)
- **`decomposers.hpp`** - dMatrix decomposition algorithms (LU, QR, Cholesky, LDLT, SVD)
- **`eigen.hpp`** - Eigenvalue and eigenvector computations

## 1. dMatrix Operations (`matrix-operations.hpp`)

### Dot Product

**Function Name:** `dot_product`

**Description:** Computes the dot product (scalar product) of two vectors.

**Parameters:**

- `const dVec& a` - First vector of size n
- `const dVec& b` - Second vector of size n

**Returns:** A double representing the dot product $a \cdot b = \sum_{i=0}^{n-1} a_i b_i$

**Example Usage:**

```C++
dVec a = {1.0, 2.0, 3.0};
dVec b = {4.0, 5.0, 6.0};
double result = dot_product(a, b); // Result: 32.0
```

### Cross Product

**Function Name:** `cross_product`

**Description:** Computes the cross product of two 3D vectors.

**Parameters:**

- `const triple& a` - First 3D vector
- `const triple& b` - Second 3D vector

**Returns:** A triple (3D vector) representing $a \times b$

**Example Usage:**

```C++
triple a = {1.0, 0.0, 0.0};
triple b = {0.0, 1.0, 0.0};
triple result = cross_product(a, b); // Result: {0.0, 0.0, 1.0}
```

### dMatrix-Vector Multiplication

**Function Name:** `matrix_multiply` (overload 1)

**Description:** Multiplies a matrix by a vector, computing $y = Ax$ where $A$ is an $n \times m$ matrix and $x$ is a vector of size $m$.

**Parameters:**

- `const dMatrix& A` - n × m matrix
- `const dVec& x` - Vector of size m

**Returns:** A vector of size n representing the product $Ax$

**Example Usage:**

```C++
dMatrix A = {{1.0, 2.0}, {3.0, 4.0}, {5.0, 6.0}}; // 3x2 matrix
dVec x = {1.0, 2.0};
dVec result = matrix_multiply(A, x); // Result: {5.0, 11.0, 17.0}
```

### dMatrix-dMatrix Multiplication

**Function Name:** `matrix_multiply` (overload 2)

**Description:** Multiplies two matrices, computing $C = AB$ where $A$ is $n \times m$ and $B$ is $m \times p$.

**Parameters:**

- `const dMatrix& A` - n × m matrix
- `const dMatrix& B` - m × p matrix

**Returns:** An n × p matrix representing the product $AB$

**Example Usage:**

```C++
dMatrix A = {{1.0, 2.0}, {3.0, 4.0}};
dMatrix B = {{5.0, 6.0}, {7.0, 8.0}};
dMatrix result = matrix_multiply(A, B); // Result: {{19.0, 22.0}, {43.0, 50.0}}
```

### Element-wise Multiplication

**Function Name:** `elementwise_multiply`

**Description:** Computes the element-wise (Hadamard) product of two matrices or vectors.

**Parameters (dMatrix overload):**

- `const dMatrix& A` - n × m matrix
- `const dMatrix& B` - n × m matrix

**Parameters (Vector overload):**

- `const dVec& a` - Vector of size n
- `const dVec& b` - Vector of size n

**Returns:** An n × m matrix or vector of size n with element-wise products

**Example Usage:**

```C++
dVec a = {1.0, 2.0, 3.0};
dVec b = {2.0, 3.0, 4.0};
dVec result = elementwise_multiply(a, b); // Result: {2.0, 6.0, 12.0}
```

### Vector and dMatrix Addition

**Function Name:** `add`

**Description:** Adds two matrices or vectors element-wise.

**Parameters (dMatrix overload):**

- `const dMatrix& A` - n × m matrix
- `const dMatrix& B` - n × m matrix

**Parameters (Vector overload):**

- `const dVec& a` - Vector of size n
- `const dVec& b` - Vector of size n

**Returns:** An n × m matrix or vector of size n with element-wise sums

**Example Usage:**

```C++
dVec a = {1.0, 2.0, 3.0};
dVec b = {4.0, 5.0, 6.0};
dVec result = add(a, b); // Result: {5.0, 7.0, 9.0}
```

### Vector and dMatrix Subtraction

**Function Name:** `subtract`

**Description:** Subtracts two matrices or vectors element-wise.

**Parameters:** Similar to `add`

**Returns:** Difference matrices or vectors

**Example Usage:**

```C++
dVec a = {5.0, 7.0, 9.0};
dVec b = {1.0, 2.0, 3.0};
dVec result = subtract(a, b); // Result: {4.0, 5.0, 6.0}
```

### Scalar Multiplication

**Function Name:** `scalar_multiply`

**Description:** Multiplies a matrix or vector by a scalar.

**Parameters (dMatrix overload):**

- `const dMatrix& A` - n × m matrix
- `double scalar` - Scalar value

**Parameters (Vector overload):**

- `const dVec& v` - Vector of size n
- `double scalar` - Scalar value

**Returns:** Scaled matrix or vector

**Example Usage:**

```C++
dVec v = {1.0, 2.0, 3.0};
dVec result = scalar_multiply(v, 2.0); // Result: {2.0, 4.0, 6.0}
```

### Vector Norm

**Function Name:** `norm`

**Description:** Computes the Euclidean norm (2-norm) of a vector.

**Parameters:**

- `const dVec& v` - Vector of size n

**Returns:** A double representing $\|v\|_2 = \sqrt{\sum_{i=0}^{n-1} v_i^2}$

**Example Usage:**

```C++
dVec v = {3.0, 4.0};
double result = norm(v); // Result: 5.0
```

## 2. dMatrix Properties (`matrix-properties.hpp`)

### Trace

**Function Name:** `trace`

**Description:** Computes the trace of a square matrix (sum of diagonal elements).

**Parameters:**

- `const dMatrix& A` - n × n square matrix

**Returns:** A double representing $\text{tr}(A) = \sum_{i=0}^{n-1} A_{i,i}$

**Example Usage:**

```C++
dMatrix A = {{1.0, 2.0}, {3.0, 4.0}};
double result = trace(A); // Result: 5.0
```

### Determinant

**Function Name:** `determinant`

**Description:** Computes the determinant of a square matrix. For matrices larger than 3×3, uses LU decomposition for efficiency.

**Parameters:**

- `const dMatrix& A` - n × n square matrix

**Returns:** A double representing $\det(A)$

**Example Usage:**

```C++
dMatrix A = {{1.0, 2.0}, {3.0, 4.0}};
double result = determinant(A); // Result: -2.0
```

### Minor

**Function Name:** `minor`

**Description:** Computes the minor of a matrix element (determinant of the submatrix obtained by removing a row and column).

**Parameters:**

- `const dMatrix& A` - n × n square matrix
- `size_t row` - Row index to remove
- `size_t col` - Column index to remove

**Returns:** A double representing the determinant of the resulting (n-1) × (n-1) submatrix

**Example Usage:**

```C++
dMatrix A = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
double result = minor(A, 0, 0); // Minor removing row 0 and column 0
```

### Transpose

**Function Name:** `transpose`

**Description:** Computes the transpose of a matrix.

**Parameters:**

- `const dMatrix& A` - n × m matrix

**Returns:** An m × n matrix representing $A^T$

**Example Usage:**

```C++
dMatrix A = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}};
dMatrix result = transpose(A); // Result: {{1.0, 4.0}, {2.0, 5.0}, {3.0, 6.0}}
```

### Cofactor dMatrix

**Function Name:** `cofactor_matrix`

**Description:** Computes the cofactor matrix of a square matrix.

**Parameters:**

- `const dMatrix& A` - n × n square matrix

**Returns:** An n × n cofactor matrix

**Example Usage:**

```C++
dMatrix A = {{1.0, 2.0}, {3.0, 4.0}};
dMatrix result = cofactor_matrix(A); // Result: {{4.0, -3.0}, {-2.0, 1.0}}
```

### Adjugate (Classical Adjoint)

**Function Name:** `adjugate`

**Description:** Computes the adjugate (classical adjoint) of a matrix, which is the transpose of the cofactor matrix.

**Parameters:**

- `const dMatrix& A` - n × n square matrix

**Returns:** An n × n adjugate matrix

**Example Usage:**

```C++
dMatrix A = {{1.0, 2.0}, {3.0, 4.0}};
dMatrix result = adjugate(A); // Result: {{4.0, -2.0}, {-3.0, 1.0}}
```

### Inverse

**Function Name:** `inverse`

**Description:** Computes the matrix inverse $A^{-1}$ such that $AA^{-1} = I$. For matrices larger than 3×3, uses LU decomposition.

**Parameters:**

- `const dMatrix& A` - n × n invertible square matrix

**Returns:** An n × n inverse matrix

**Example Usage:**

```C++
dMatrix A = {{1.0, 2.0}, {3.0, 4.0}};
dMatrix result = inverse(A); // Inverse of A
```

### Rank

**Function Name:** `rank`

**Description:** Computes the rank of a matrix using QR decomposition.

**Parameters:**

- `const dMatrix& A` - n × m matrix
- `double tol` - Tolerance for considering a diagonal element as zero (default: 1e-10)

**Returns:** A size_t representing the rank

**Example Usage:**

```C++
dMatrix A = {{1.0, 2.0}, {2.0, 4.0}, {3.0, 6.0}};
size_t r = rank(A); // Result: 1 (rank-deficient)
```

### Condition Number

**Function Name:** `condition_number`

**Description:** Computes the condition number of a matrix in the 2-norm, indicating the sensitivity of the matrix to perturbations.

**Parameters:**

- `const dMatrix& A` - n × n invertible square matrix

**Returns:** A double representing the condition number $\kappa(A) = \|A\| \|A^{-1}\|$

**Example Usage:**

```C++
dMatrix A = {{1.0, 2.0}, {3.0, 4.0}};
double cond = condition_number(A);
```

## 3. dMatrix Decompositions (`decomposers.hpp`)

### LU Decomposition

**Function Name:** `lu_decomposition`

**Description:** Performs LU decomposition with partial pivoting, decomposing a square matrix $A$ into $PA = LU$, where $P$ is a permutation matrix, $L$ is lower triangular, and $U$ is upper triangular.

**Parameters:**

- `dMatrix A` - n × n square matrix

**Returns:** A tuple `{L, U, P}` where:

- `dMatrix L` - Lower triangular matrix with ones on the diagonal
- `dMatrix U` - Upper triangular matrix
- `wVec P` - Permutation vector

**Example Usage:**

```C++
dMatrix A = {{4.0, 3.0}, {6.0, 3.0}};
auto [L, U, P] = lu_decomposition(A);
// L = {{1.0, 0.0}, {1.5, 1.0}}
// U = {{4.0, 3.0}, {0.0, -1.5}}
// P = {0, 1}
```

### QR Decomposition

**Function Name:** `qr_decomposition`

**Description:** Performs QR decomposition using modified Gram-Schmidt process, decomposing an m × n matrix $A$ into $A = QR$, where $Q$ is orthonormal and $R$ is upper triangular.

**Parameters:**

- `const dMatrix& A` - m × n matrix

**Returns:** A pair `{Q, R}` where:

- `dMatrix Q` - m × n matrix with orthonormal columns
- `dMatrix R` - n × n upper triangular matrix

**Example Usage:**

```C++
dMatrix A = {{1.0, 1.0}, {1.0, 0.0}, {0.0, 1.0}};
auto [Q, R] = qr_decomposition(A);
```

---

### Cholesky Decomposition

**Function Name:** `cholesky_decomposition`

**Description:** Performs Cholesky decomposition for symmetric positive definite matrices, decomposing $A = LL^T$ where $L$ is lower triangular.

**Parameters:**

- `const dMatrix& A` - n × n symmetric positive definite matrix

**Returns:** A matrix `L` representing the lower triangular factor

**Example Usage:**

```C++
dMatrix A = {{4.0, 2.0}, {2.0, 3.0}};
dMatrix L = cholesky_decomposition(A);
// A = L * L^T
```

### LDLT Decomposition

**Function Name:** `ldlt_decomposition`

**Description:** Performs LDLT decomposition for symmetric matrices, decomposing $A = LDL^T$ where $L$ is lower triangular with ones on the diagonal and $D$ is diagonal.

**Parameters:**

- `const dMatrix& A` - n × n symmetric matrix

**Returns:** A pair `{L, D}` where:

- `dMatrix L` - Lower triangular matrix with ones on the diagonal
- `dVec D` - Diagonal vector

**Example Usage:**

```C++
dMatrix A = {{4.0, 2.0}, {2.0, 3.0}};
auto [L, D] = ldlt_decomposition(A);
```

### SVD Decomposition

**Function Name:** `svd_decomposition`

**Description:** Performs Singular Value Decomposition using eigenvalue decomposition, decomposing an m × n matrix $A$ into $A = U \Sigma V^T$, where $U$ and $V$ have orthonormal columns and $\Sigma$ is diagonal with non-negative singular values.

**Parameters:**

- `const dMatrix& A` - m × n matrix
- `size_t max_iter` - Maximum iterations for eigenvalue computation (default: 100)
- `double tol` - Convergence tolerance (default: 1e-10)

**Returns:** A tuple `{U, singular_values, VT}` where:

- `dMatrix U` - m × min(m,n) orthonormal matrix
- `dVec singular_values` - Vector of singular values in descending order
- `dMatrix VT` - min(m,n) × n matrix

**Example Usage:**

```C++
dMatrix A = {{1.0, 2.0}, {3.0, 4.0}};
auto [U, S, VT] = svd_decomposition(A);
```

## 4. Eigenvalue Computation (`eigen.hpp`)

### Power Method

**Function Name:** `power_method`

**Description:** Computes the dominant eigenvalue and corresponding eigenvector of a square matrix using the power iteration method.

**Parameters:**

- `const dMatrix& A` - n × n square matrix
- `size_t max_iter` - Maximum number of iterations (default: 100)
- `double tol` - Convergence tolerance (default: 1e-10)

**Returns:** A pair `{eigenvalue, eigenvector}` where:

- `double eigenvalue` - The dominant (largest) eigenvalue
- `dVec eigenvector` - Corresponding normalized eigenvector

**Example Usage:**

```C++
dMatrix A = {{3.0, 1.0}, {1.0, 2.0}};
auto [lambda, v] = power_method(A);
// lambda is the largest eigenvalue
// v is the corresponding eigenvector
```

### Hessenberg Reduction

**Function Name:** `to_hessenberg`

**Description:** Reduces a square matrix to upper Hessenberg form using Householder reflections. This is often a preprocessing step for QR iteration.

**Parameters:**

- `dMatrix A` - n × n square matrix

**Returns:** An n × n matrix in upper Hessenberg form

**Example Usage:**

```C++
dMatrix A = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
dMatrix H = to_hessenberg(A);
```

### QR Eigenvalue Algorithm

**Function Name:** `eigen_qr`

**Description:** Computes all eigenvalues and eigenvectors of a square matrix using QR iteration with shifts (QR algorithm).

**Parameters:**

- `dMatrix A` - n × n square matrix
- `size_t max_iter` - Maximum number of QR iterations (default: 100)
- `double tol` - Convergence tolerance (default: 1e-10)

**Returns:** A pair `{eigenvalues, eigenvectors}` where:

- `dVec eigenvalues` - Vector of n eigenvalues
- `dMatrix eigenvectors` - dMatrix with eigenvectors as columns

**Example Usage:**

```C++
dMatrix A = {{3.0, 1.0}, {1.0, 2.0}};
auto [eigenvalues, eigenvectors] = eigen_qr(A);
// eigenvalues: all eigenvalues of A
// eigenvectors: columns are corresponding eigenvectors
```

## Interdependencies

The decomposer module includes `matrix-operations.hpp` and `eigen.hpp`, and `matrix-properties.hpp` includes `decomposers.hpp`. This structure allows for efficient use of decompositions in computing matrix properties.

For further details on implementations, refer to the header file comments in the respective files.
