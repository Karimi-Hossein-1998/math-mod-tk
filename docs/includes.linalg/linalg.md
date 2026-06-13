# Linear Algebra Module (`linalg/`)

The Linear Algebra (linalg) module is a comprehensive collection of header-only linear algebra utilities designed for mathematical modeling and numerical computing. It provides efficient implementations of matrix operations, decomposition algorithms, and linear system solvers, all while maintaining minimal external dependencies.

## Table of Contents

1. [Linear Algebra Module (`linalg/`)](#linear-algebra-module-linalg)
    1. [Table of Contents](#table-of-contents)
    2. [Module Organization](#module-organization)
        1. [1. **Basics** (`basics/`)](#1-basics-basics)
        2. [2. **Solvers** (`solvers/`)](#2-solvers-solvers)
    3. [Quick Start](#quick-start)
        1. [dMatrix Operations](#dmatrix-operations)
        2. [dMatrix Decomposition](#dmatrix-decomposition)
        3. [Eigenvalues and Eigenvectors](#eigenvalues-and-eigenvectors)
        4. [Solving Linear Systems](#solving-linear-systems)
    4. [Key Features](#key-features)
        1. [1. **Header-Only Library**](#1-header-only-library)
        2. [2. **Minimal Dependencies**](#2-minimal-dependencies)
        3. [3. **Flexible dMatrix Representation**](#3-flexible-dmatrix-representation)
        4. [4. **Error Handling**](#4-error-handling)
        5. [5. **Algorithm Selection**](#5-algorithm-selection)
        6. [6. **Specialized Solvers**](#6-specialized-solvers)
    5. [Algorithm Complexity Summary](#algorithm-complexity-summary)
    6. [Numerical Stability](#numerical-stability)
    7. [Use Cases](#use-cases)
        1. [Scientific Computing](#scientific-computing)
        2. [Linear Algebra Problems](#linear-algebra-problems)
        3. [Optimization](#optimization)
        4. [Network Analysis](#network-analysis)
    8. [Complete Documentation](#complete-documentation)
    9. [Integration with Other Modules](#integration-with-other-modules)
    10. [Performance Tips](#performance-tips)
    11. [Future Enhancements](#future-enhancements)

## Module Organization

The linalg module is organized into two main directories:

### 1. **Basics** (`basics/`)

Core linear algebra operations and matrix decompositions.

- [**dMatrix Operations**](./basics/basics.md#1-matrix-operations-matrix-operationshpp)
  - Dot product, cross product
  - dMatrix-vector and matrix-matrix multiplication
  - Element-wise operations and scaling
  - Vector norms

- [**dMatrix Properties**](./basics/basics.md#2-matrix-properties-matrix-propertieshpp)
  - Trace, determinant, rank
  - Transpose, inverse
  - Cofactor matrix and adjugate
  - Condition number

- [**dMatrix Decompositions**](./basics/basics.md#3-matrix-decompositions-decomposershpp)
  - LU decomposition (with partial pivoting)
  - QR decomposition (Gram-Schmidt)
  - Cholesky decomposition
  - LDLT decomposition
  - SVD decomposition

- [**Eigenvalue Computation**](./basics/basics.md#4-eigenvalue-computation-eigenhpp)
  - Power method
  - QR algorithm
  - Hessenberg reduction

### 2. **Solvers** (`solvers/`)

Specialized algorithms for solving linear systems $Ax = b$.

- [**General Methods**](./solvers/solvers.md)
  - Gaussian elimination with partial pivoting
  - LU-based solver
  - QR-based solver
  - SVD-based solver

- [**Specialized Methods**](./solvers/solvers.md)
  - Cholesky solver (for symmetric positive definite matrices)
  - LDLT solver (for symmetric matrices)
  - Tridiagonal solver (Thomas algorithm)
  - Banded matrix solver

## Quick Start

### dMatrix Operations

```C++
#include "includes/linalg/basics/matrix-operations.hpp"

// Vectors
dVec a = {1.0, 2.0, 3.0};
dVec b = {4.0, 5.0, 6.0};
double dot = dot_product(a, b);  // 32.0
dVec sum = add(a, b);             // {5.0, 7.0, 9.0}

// Matrices
dMatrix A = {{1.0, 2.0}, {3.0, 4.0}};
dMatrix B = {{5.0, 6.0}, {7.0, 8.0}};
dMatrix C = matrix_multiply(A, B);
dVec x = {1.0, 2.0};
dVec y = matrix_multiply(A, x);
```

### dMatrix Decomposition

```C++
#include "includes/linalg/basics/decomposers.hpp"

dMatrix A = {{4.0, 3.0}, {6.0, 3.0}};

// LU Decomposition
auto [L, U, P] = lu_decomposition(A);

// QR Decomposition
auto [Q, R] = qr_decomposition(A);

// Cholesky Decomposition (for SPD matrices, here A is one such matrix) SPD = strictly positive definite 
dMatrix L_chol = cholesky_decomposition(A);

// SVD Decomposition
auto [U, S, VT] = svd_decomposition(A);
```

### Eigenvalues and Eigenvectors

```C++
#include "includes/linalg/basics/eigen.hpp"

dMatrix A = {{3.0, 1.0}, {1.0, 2.0}};

// Dominant eigenvalue and eigenvector (power method)
auto [lambda, v] = power_method(A);

// All eigenvalues and eigenvectors (QR algorithm)
auto [eigenvalues, eigenvectors] = eigen_qr(A);
```

### Solving Linear Systems

```C++
#include "includes/linalg/solvers/lu-solve.hpp"

dMatrix A = {{4.0, 3.0}, {6.0, 3.0}};
dVec b = {24.0, 33.0};

// LU solver
dVec x = lu_solve(A, b);

// For symmetric positive definite systems
#include "includes/linalg/solvers/llt-solve.hpp"
dVec x_spd = llt_solve(A, b);

// For tridiagonal systems
/* A = 
    2.0 1.0 0.0
    1.0 2.0 1.0
    0.0 1.0 2.0

a = 1.0 1.0
diag = 2.0 2.0 2.0 
c = 1.0 1.0
*/
#include "includes/linalg/solvers/tridiagonal-solver.hpp"
dVec a = {1.0, 1.0};
dVec diag = {2.0, 2.0, 2.0};
dVec c = {1.0, 1.0};
dVec x_tri = tridiagonal_solve(a, diag, c, b);
```

## Key Features

### 1. **Header-Only Library**

All implementations are in headers, allowing easy integration without compilation or linking.

### 2. **Minimal Dependencies**

Uses only standard C++ library (`<vector>`, `<cmath>`, `<stdexcept>`, etc.).

### 3. **Flexible dMatrix Representation**

Matrices are represented as `dMatrix = std::vector<std::vector<double>>`, providing flexibility and easy integration.

### 4. **Error Handling**

Comprehensive error checking with informative `std::invalid_argument` and `std::runtime_error` exceptions.

### 5. **Algorithm Selection**

- **LU Decomposition:** General-purpose with partial pivoting
- **QR Decomposition:** Better for ill-conditioned systems
- **Cholesky:** Fastest for symmetric positive definite matrices (2× faster than LU)
- **SVD:** For singular and near-singular systems

### 6. **Specialized Solvers**

- **Tridiagonal Solver:** Linear time complexity $O(n)$ for tridiagonal systems
- **Banded Solver:** Efficient for banded matrices with small bandwidth
- **Cholesky/LDLT:** Optimized for symmetric matrices

## Algorithm Complexity Summary

```text
| Operation | Time Complexity | Space Complexity | Notes |
|-----------|-----------------|------------------|-------|
| Vector dot product | $O(n)$ | $O(1)$ | - |
| dMatrix-vector multiply | $O(n^2)$ | $O(1)$ | For n×n matrix |
| dMatrix-matrix multiply | $O(n^3)$ | $O(n^2)$ | Dense algorithm |
| LU Decomposition | $O(n^3)$ | $O(n^2)$ | With partial pivoting |
| QR Decomposition | $O(n^3)$ | $O(n^2)$ | Gram-Schmidt |
| Cholesky | $O(n^3)$ | $O(n^2)$ | ~2× faster than LU |
| Tridiagonal solve | $O(n)$ | $O(n)$ | Thomas algorithm |
| Banded solve | $O(n \cdot (kl+ku)^2)$ | $O(n(kl+ku))$ | kl, ku = bandwidths |
| SVD | $O(n^3)$ | $O(n^2)$ | Jacobi method |
| Determinant | $O(n^3)$ | $O(n^2)$ | Uses LU for n > 3 |
| Inverse | $O(n^3)$ | $O(n^2)$ | Uses LU for n > 3 |
| Eigenvalues (QR) | $O(n^3)$ per iteration | $O(n^2)$ | Iterative method |
```

## Numerical Stability

The module implements several strategies for numerical stability:

1. **Partial Pivoting:** Used in LU decomposition to avoid division by small numbers
2. **Gram-Schmidt (QR):** Orthogonalization prevents numerical issues
3. **Cholesky:** Exploits positive definiteness for stability
4. **SVD:** Naturally handles singular and near-singular matrices via threshold
5. **Error Checking:** All functions check for dimension mismatches and singular matrices

---

## Use Cases

### Scientific Computing

- Solving systems of linear differential equations
- Finite element method (FEM) implementations
- Computational mechanics and dynamics

### Linear Algebra Problems

- Eigenvalue analysis
- Principal Component Analysis (PCA)
- Least-squares solutions

### Optimization

- Gradient-based methods
- Constraint handling
- Hessian computations

### Network Analysis

- Adjacency matrix operations (used in Kuramoto model)
- Spectral methods
- Graph Laplacian computations

---

## Complete Documentation

For detailed information on specific functions, parameters, and examples, refer to:

- [**Basics Module Documentation**](./basics/basics.md) - dMatrix operations, properties, and decompositions
- [**Solvers Module Documentation**](./solvers/solvers.md) - Linear system solvers and their selection guide

## Integration with Other Modules

The linalg module is self-contained and can be used independently. However, it integrates seamlessly with:

- **`models/`** - Computing Jacobians and system matrices
- **`solvers/`** - Providing linear algebra support for numerical methods
- **`utility/`** - General computational utilities

## Performance Tips

1. **Choose the right solver:** Always exploit matrix structure (symmetry, bandedness, sparsity)
2. **Avoid copies:** The solvers modify their arguments, consider using references if possible
3. **Precompute decompositions:** If solving multiple systems with the same matrix, decompose once
4. **Use appropriate data structures:** For sparse matrices, custom sparse implementations would be beneficial (future enhancement)
5. **Monitor condition numbers:** Check if your system is ill-conditioned before choosing a solver

## Future Enhancements

Potential additions to improve the module:

1. **Sparse matrix support** - Efficient storage and computation for sparse matrices
2. **Iterative solvers** - Conjugate Gradient, GMRES for large systems
3. **Sparse direct solvers** - UMFPACK-like approaches
4. **BLAS/LAPACK bindings** - Optional acceleration for larger systems
5. **Parallelization** - Multi-threading for dense operations
6. **GPU acceleration** - CUDA/OpenCL kernels for large-scale problems
