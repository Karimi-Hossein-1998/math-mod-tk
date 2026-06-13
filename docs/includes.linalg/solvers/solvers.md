# Linear System Solvers (`solvers/`)

This directory contains headers for solving linear systems of equations $Ax = b$ using various decomposition and direct solution methods. Each solver is optimized for specific matrix structures and properties.

## Table of Contents

<!-- markdownlint-disable MD051 -->
1. [Linear System Solvers (`solvers/`)](#linear-system-solvers-solvers)
    1. [Table of Contents](#table-of-contents)
    2. [Overview](#overview)
    3. [1. Gaussian Elimination](#1-gaussian-elimination)
    4. [2. LU Solver](#2-lu-solver)
    5. [3. QR Solver](#3-qr-solver)
    6. [4. Cholesky Solver (LLT)](#4-cholesky-solver-llt)
    7. [5. LDLT Solver](#5-ldlt-solver)
    8. [6. Tridiagonal Solver (Thomas Algorithm)](#6-tridiagonal-solver-thomas-algorithm)
    9. [7. Banded dMatrix Solver](#7-banded-matrix-solver)
    10. [8. SVD Solver](#8-svd-solver)
    11. [Solver Selection Guide](#solver-selection-guide)
    12. [Performance Considerations](#performance-considerations)
    13. [Error Handling](#error-handling)
<!-- markdownlint-enable MD051 -->
## Overview

The solvers module provides the following methods:

- **`gaussian-elimination.hpp`** - Gaussian elimination with partial pivoting
- **`lu-solve.hpp`** - LU factorization-based solver
- **`qr-solve.hpp`** - QR decomposition-based solver
- **`llt-solve.hpp`** - Cholesky decomposition-based solver (for symmetric positive definite matrices)
- **`ldlt-solve.hpp`** - LDLT decomposition-based solver (for symmetric matrices)
- **`tridiagonal-solver.hpp`** - Thomas algorithm for tridiagonal systems
- **`banded-solver.hpp`** - Specialized solver for banded matrices
- **`svd-solve.hpp`** - SVD-based solver

## 1. Gaussian Elimination

**Function Name:** `gaussian_elimination`

**Description:** Solves a linear system $Ax = b$ using Gaussian elimination with partial pivoting. This is a general-purpose direct solver suitable for most square systems.

**Parameters:**

- `dMatrix A` - n × n coefficient matrix (passed by value)
- `dVec b` - Right-hand side vector of size n (passed by value)

**Returns:** A vector `x` of size n containing the solution

**Numerical Stability:** Partial pivoting ensures good numerical stability by selecting the largest available pivot at each step.

**Time Complexity:** $O(n^3)$

**Example Usage:**

```C++
dMatrix A = {{4.0, 3.0}, {6.0, 3.0}};
dVec b = {24.0, 33.0};
dVec x = gaussian_elimination(A, b);
// x should satisfy A*x = b
```

**Note:** Modifies copies of `A` and `b`, leaving originals unchanged.

## 2. LU Solver

**Function Name:** `lu_solve`

**Description:** Solves a linear system $Ax = b$ using LU decomposition with partial pivoting. This method computes the factorization $PA = LU$ and then solves through forward and backward substitution.

**Parameters:**

- `dMatrix A` - n × n coefficient matrix (passed by value)
- `dVec b` - Right-hand side vector of size n (passed by value)

**Returns:** A vector `x` of size n containing the solution

**Numerical Stability:** Partial pivoting ensures numerical stability. The method is equivalent to Gaussian elimination but more structured.

**Time Complexity:** $O(n^3)$

**Example Usage:**

```C++
dMatrix A = {{4.0, 3.0}, {6.0, 3.0}};
dVec b = {24.0, 33.0};
dVec x = lu_solve(A, b);
```

**Advantage:** When multiple right-hand sides need to be solved with the same matrix, the LU decomposition can be computed once and reused.

## 3. QR Solver

**Function Name:** `qr_solve`

**Description:** Solves a linear system $Ax = b$ using QR decomposition via the Gram-Schmidt orthogonalization process. The system is reduced to $Rx = Q^T b$ and solved via back substitution.

**Parameters:**

- `dMatrix A` - n × n coefficient matrix (passed by value)
- `dVec b` - Right-hand side vector of size n (passed by value)

**Returns:** A vector `x` of size n containing the solution

**Numerical Stability:** QR decomposition provides good numerical stability, particularly for ill-conditioned systems, though at higher computational cost than LU.

**Time Complexity:** $O(n^3)$

**Example Usage:**

```C++
dMatrix A = {{1.0, 1.0}, {1.0, 2.0}, {1.0, 3.0}};
dVec b = {1.0, 2.0, 3.0};
dVec x = qr_solve(A, b);
```

**Advantage:** Better conditioned than Gaussian elimination for nearly singular systems. Also suitable for overdetermined systems (least squares problems).

## 4. Cholesky Solver (LLT)

**Function Name:** `llt_solve`

**Description:** Solves a linear system $Ax = b$ for symmetric positive definite matrices using Cholesky decomposition $A = LL^T$. The system is solved in two steps: forward substitution on $Ly = b$, then backward substitution on $L^T x = y$.

**Parameters:**

- `dMatrix A` - n × n symmetric positive definite matrix (passed by value)
- `dVec b` - Right-hand side vector of size n (passed by value)

**Returns:** A vector `x` of size n containing the solution

**Prerequisites:** dMatrix must be symmetric and positive definite. No pivoting is performed.

**Numerical Stability:** Excellent for positive definite matrices. Fails gracefully if positive definiteness is violated.

**Time Complexity:** $O(n^3)$, with lower constant factor than LU

**Example Usage:**

```C++
// Symmetric positive definite matrix
dMatrix A = {{4.0, 2.0}, {2.0, 3.0}};
dVec b = {2.0, 1.0};
dVec x = llt_solve(A, b);
```

**Note:** Approximately twice as fast as LU decomposition for the same matrix size, due to the special structure.

## 5. LDLT Solver

**Function Name:** `ldlt_solve`

**Description:** Solves a linear system $Ax = b$ for symmetric matrices using LDLT decomposition $A = LDL^T$, where $L$ is lower triangular with ones on the diagonal and $D$ is diagonal. The system is solved through three steps: forward substitution ($Ly = b$), diagonal scaling ($Dz = y$), and backward substitution ($L^T x = z$).

**Parameters:**

- `dMatrix A` - n × n symmetric matrix (passed by value)
- `dVec b` - Right-hand side vector of size n (passed by value)

**Returns:** A vector `x` of size n containing the solution

**Prerequisites:** dMatrix must be symmetric. Works for indefinite symmetric matrices (unlike Cholesky which requires positive definiteness).

**Numerical Stability:** Good stability for symmetric matrices. No pivoting, but diagonal structure helps.

**Time Complexity:** $O(n^3)$

**Example Usage:**

```C++
// Symmetric indefinite matrix
dMatrix A = {{2.0, 1.0}, {1.0, -1.0}};
dVec b = {3.0, 1.0};
dVec x = ldlt_solve(A, b);
```

**Advantage:** Works with indefinite symmetric matrices, unlike Cholesky. More stable than LU for symmetric matrices.

## 6. Tridiagonal Solver (Thomas Algorithm)

**Function Name:** `tridiagonal_solve`

**Description:** Solves a tridiagonal linear system using the Thomas algorithm (specialized form of Gaussian elimination). A tridiagonal matrix has non-zero elements only on the main diagonal and the two adjacent diagonals.

**Parameters:**

- `const dVec& a` - Subdiagonal vector (elements below main diagonal), size n-1
- `const dVec& b` - Main diagonal vector, size n
- `const dVec& c` - Superdiagonal vector (elements above main diagonal), size n-1
- `const dVec& d` - Right-hand side vector, size n

**Returns:** A vector `x` of size n containing the solution

**dMatrix Structure:** The matrix $A$ is represented as:
$$A = \begin{pmatrix}
b_0 & c_0 & 0 & \cdots & 0 \\
a_0 & b_1 & c_1 & \cdots & 0 \\
0 & a_1 & b_2 & \cdots & 0 \\
\vdots & \vdots & \vdots & \ddots & \vdots \\
0 & 0 & 0 & \cdots & b_{n-1}
\end{pmatrix}$$

**Time Complexity:** $O(n)$ - extremely efficient for large tridiagonal systems

**Example Usage:**

```C++
dVec a = {1.0, 1.0};        // subdiagonal
dVec b = {2.0, 2.0, 2.0};   // main diagonal
dVec c = {1.0, 1.0};        // superdiagonal
dVec d = {3.0, 4.0, 3.0};   // RHS

dVec x = tridiagonal_solve(a, b, c, d);
```

**Use Cases:** Finite difference schemes, boundary value problems, spline interpolation

**Note:** This is the most efficient solver when the matrix is tridiagonal. Computational cost is linear in the system size.

## 7. Banded dMatrix Solver

**Function Name:** `banded_solve`

**Description:** Solves a linear system $Ax = b$ where $A$ is a banded matrix (non-zero elements confined to a diagonal band). Uses Gaussian elimination adapted for banded storage format.

**Parameters:**
- `dMatrix ab` - Banded matrix in compact storage format (2*kl+ku+1) × n
- `int kl` - Lower bandwidth (number of non-zero sub-diagonals)
- `int ku` - Upper bandwidth (number of non-zero super-diagonals)
- `dVec b` - Right-hand side vector of size n

**Returns:** A vector `x` of size n containing the solution

**Storage Format:** The banded matrix is stored compactly where `ab[kl+ku+i-j][j] == A[i][j]` for valid indices. This format reduces memory usage significantly.

**Time Complexity:** $O(n(kl+ku)^2)$ - much better than $O(n^3)$ for small bandwidths

**Example Usage:**

```C++
// Pentadiagonal system (kl=2, ku=2)
// Full matrix would be:
// 2  1  0  0  0
// 1  2  1  0  0
// 0  1  2  1  0
// 0  0  1  2  1
// 0  0  0  1  2

dMatrix ab(5, dVec(5));  // (2*2+2+1) x 5 = 5 x 5
// Fill ab with compact banded format...

dVec b = {1.0, 2.0, 3.0, 4.0, 5.0};
dVec x = banded_solve(ab, 2, 2, b);
```

**Use Cases:** Finite element methods, spline computations, systems arising from finite difference discretization of PDEs

**Advantage:** Significant memory savings and computational speedup compared to dense methods when bandwidth is small relative to matrix size.

## 8. SVD Solver

**Function Name:** `svd_solve`

**Description:** Solves a linear system $Ax = b$ using Singular Value Decomposition. The matrix is decomposed as $A = U\Sigma V^T$, and the solution is computed as $x = V\Sigma^{-1}U^T b$, where singular values below a tolerance are treated as zero.

**Parameters:**
- `dMatrix A` - n × n coefficient matrix (passed by value)
- `dVec b` - Right-hand side vector of size n (passed by value)

**Returns:** A vector `x` of size n containing the solution (or pseudoinverse solution for singular systems)

**Numerical Stability:** Excellent for ill-conditioned and singular systems. Automatically handles near-singularity via threshold.

**Time Complexity:** $O(n^3)$ with higher constant than LU/Gaussian elimination

**Example Usage:**

```C++
dMatrix A = {{1.0, 1.0}, {1.0, 1.0000001}};  // Nearly singular
dVec b = {2.0, 2.0};
dVec x = svd_solve(A, b);  // Computed using pseudoinverse
```

**Advantages:**
- Works with singular and near-singular matrices
- Provides minimal norm solution for underdetermined systems
- Excellent numerical stability
- Reveals matrix rank implicitly

**Disadvantages:**
- Slower than other methods due to higher computational complexity
- Best used when system is known to be ill-conditioned or singular

## Solver Selection Guide

```text
| Solver | dMatrix Type | Advantages | When to Use |
|--------|-------------|-----------|------------|
| **Gaussian Elimination** | General | Fast, simple | Default choice for general systems |
| **LU** | General | Efficient for multiple RHS | Multiple right-hand sides |
| **QR** | General, tall | More stable | Ill-conditioned systems |
| **Cholesky (LLT)** | SPD | Fastest for SPD | Symmetric positive definite (e.g., covariance matrices) |
| **LDLT** | Symmetric | Works for indefinite | Symmetric indefinite systems |
| **Tridiagonal** | Tridiagonal | $O(n)$ time | Discretized PDEs, splines |
| **Banded** | Banded | $O(n)$ for small bandwidth | Finite elements, banded structures |
| **SVD** | Any (even singular) | Handles singularity | Singular/ill-conditioned systems |
```

## Performance Considerations

1. **Problem Size:** For $n < 100$, all methods are fast. Differences become significant for larger matrices.

2. **dMatrix Structure:** Always exploit structure:
   - Tridiagonal → `tridiagonal_solve`
   - Banded → `banded_solve`
   - Symmetric positive definite → `llt_solve`
   - Symmetric → `ldlt_solve`

3. **Conditioning:** Use QR or SVD for ill-conditioned systems; LU/Gaussian for well-conditioned.

4. **Multiple RHS:** Consider computing the decomposition once and solving multiple times if the same matrix is used repeatedly.

5. **Parallelization:** None of the current implementations are parallelized, but this could be added for large dense systems.

## Error Handling

All solvers perform dimension checking and throw `std::invalid_argument` if sizes don't match. They also check for singular/nearly singular matrices and throw `std::runtime_error` when appropriate.

Example:
```C++
try
{
    dVec x = gaussian_elimination(A, b);
}
catch (const std::invalid_argument& e)
{
    std::cerr << "Dimension mismatch: " << e.what() << std::endl;
}
catch (const std::runtime_error& e)
{
    std::cerr << "Singular matrix: " << e.what() << std::endl;
}
```
