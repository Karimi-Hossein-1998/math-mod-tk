# Mathematical Modelling Framework

THis is a project for a header-only C++ mathematical modelling framework focused on mathematical accuracy, performance and speed, fair ease of use, and modern C++ standards (and occasionally some non-standard but widely supported dependencies).

It is made in two main parts:

- **`includes`** - The main library headers (actual models, solvers, utilities, etc.)
- **`Examples`** - Example use-cases and applications of the library. Even some works similar to my M.Sc. thesis.

## Table of Contents

1. [Mathematical Modelling Framework](#mathematical-modelling-framework)
    1. [Table of Contents](#table-of-contents)
    2. [Core Practices](#core-practices)
    3. [Core Definitions and Notations](#core-definitions-and-notations)
        1. [`#include` Management Policy](#include-management-policy)
        2. [Numerics](#numerics)
        3. [Aliases and Abbreviations](#aliases-and-abbreviations)
        4. [`struct`s For Data-Structure](#structs-for-data-structure)
    4. [The Library (`includes`)](#the-library-includes)
        1. [Initializers](#initializers)
        2. [interpolators](#interpolators)
        3. [linalg](#linalg)
        4. [miscellany](#miscellany)
        5. [models](#models)
            1. [Notes On Handling Models](#notes-on-handling-models)
        6. [namespaces](#namespaces)
        7. [network](#network)
            1. [Function List](#function-list)
        8. [solvers](#solvers)
        9. [typedefs](#typedefs)
        10. [utility](#utility)
    5. [Compilation Notes](#compilation-notes)

```text
#  - [typedefs](#typedefs)
#  - [utility](#utility)
```

## [Core Practices](includes/typedefs/header.hpp)

- **Header-Only** - The library is designed to be header-only, meaning all functionalities are implemented in header files. This allows for easy integration into existing projects without the need for separate compilation or linking steps (preventing further complications).
- **Modern C++ Standards** - The library adheres to modern C++ standards (C++14 and above), utilizing features such as smart pointers, lambda expressions, and the Standard Template Library (STL) to enhance code readability, maintainability, and performance.
- **Performance Optimization** - The library is optimized for performance (to the best of my knowledge), with careful consideration given to algorithmic efficiency and memory management. This includes the use of efficient data structures, minimizing unnecessary copies, and leveraging multi-threading where appropriate.
- **Comprehensive Documentation** - The library is thoroughly documented, with clear explanations of each function, class, and data structure. This includes usage examples, parameter descriptions, and return value explanations to facilitate ease of use for developers.
- **Extensibility** - The library is designed to be easily extensible, allowing users to add new models, solvers, and utilities as needed. This is achieved through a modular architecture and clear guidelines for extending the library's functionality.
- **Clean Code Aesthetics** - The codebase follows clean coding principles, with consistent naming conventions, modular functions, and clear separation of concerns. This enhances code readability and maintainability, making it easier for developers to understand and contribute to the project. Also, usage of vertical alignments, indentations, comments, and spacing to improve readability.
- **Error Handling** - The library incorporates robust error handling mechanisms to ensure that users are informed of any issues that arise during execution. This includes the use of exceptions, assertions, and informative error messages to aid in debugging and troubleshooting. In some cases the user needs to check for any mismatches using separate functions (*validators*) before using the solvers and alike.

## Core Definitions and Notations

The main entry in all header files is **`includes/typedefs/header.hpp`** which has the aliases and most used type definitions (structs) for the whole system. If you wish to add new feature that might use a certain data-structure, make sure to define the structures here (data-structures for models are defined on the spot for simplicity and ease of use).

**Includes** are solely managed through this header. If you wish to add a new feature that has repeated use and needs new includes you may expand the entries. Current list of includes are:

### `#include` Management Policy

```C++
#include <cassert>
#include <vector>
#include <thread>
#include <array>
#include <functional>
#include <cmath>
#include <iostream>
#include <random>
#include <utility>
#include <algorithm>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <ostream>
```

These are all standard **C++** libraries.`<cassert>` for the `assert` functionality, `<vector>` for dynamic arrays, `<thread>` for multi-threading support, `<array>` for fixed-size arrays, `<functional>` for function objects and lambdas, `<cmath>` for mathematical functions, `<iostream>` for input/output operations, `<random>` for random number generation, `<utility>` for utility functions like `std::pair`, `<algorithm>` for algorithms like `std::sort`, `<string>` for string manipulations, `<fstream>` for file handling, `<iomanip>` for formatted I/O, `<sstream>` for string streams, `<stdexcept>` for standard exceptions, and `<ostream>` for output stream operations.

### Numerics

There are some Global numbers that have repeated usage across the *codebase*:

```C++
constexpr double PI        = 3.14159265358979323846;
constexpr int    max_order = 10;
```

- **PI** - The mathematical constant π (pi), approximately equal to 3.14159. It is defined as the ratio of a circle's circumference to its diameter and is widely used in various mathematical and scientific calculations.

```C++
constexpr double PI        = 3.14159265358979323846;
```

- **max_order** - This constant defines the maximum order for certain numerical methods, particularly in the context of multistep ODE (and DDE) solvers like Adams-Bashforth-Moulton (ABM) methods. It is set to 10, indicating that the library supports methods up to the $10^{th}$ order (i.e. the coefficients are pre-calculated up-to-the $10^{th}$ order, there is no obligation to be limited to them. You can use the script to generate coefficients for higher order ABM solver, be careful with their implementation!).

```C++
constexpr int    max_order = 10;
```

### Aliases and Abbreviations

There are aliases defined for convenience, each serving a specific purpose:

```C++
template<typename T>                                                              // A template for std::vector
using Vec            = std::vector<T>;                                            // that is basically a renaming (Vec = std::vector)
template<typename T>
using Matrix         = std::vector<std::vector<T>>;
using dVec           = Vec<double>;                                               // 1D vector of doubles
using wVec           = Vec<size_t>;
using dcVec          = Vec<std::complex<double>>;                                   // 1D vector of complex doubles
using iVec           = Vec<int>;                                                  // 1D vector of integers
using uVec           = Vec<unsigned int>;                                         // 1D vector of unsigned integers
using bVec           = Vec<bool>;                                                 // 1D vector of booleans
using sVec           = Vec<std::string>;                                              // 1D vector of strings
using dpairVec       = Vec<std::pair<double, double>>;                             // 1D vector of (double, double) pairs
using spairVec       = Vec<std::pair<std::string, std::string>>;                     // 1D vector of (string, string) pairs
using triple         = std::array<double, 3>;
using dMatrix        = Vec<dVec >;                                                // 2D matrix (vector of vectors)
using dcMatrix       = Matrix<std::complex<double>>;                              // 2D matrix (vector of vectors) -- std::complex
using wMatrix        = Matrix<size_t>;
using iMatrix        = Matrix<int>;
using uMatrix        = Matrix<unsigned int>;
using bMatrix        = Matrix<bool>;
using sMatrix        = Matrix<std::string>;
using func           = std::function<double(double)>;                             // Function type taking double and returning double
using Func           = std::function<dVec(double)>;                               // Function type taking double and returning dVec
using MyFunc         = std::function<dVec(double, const dVec&)>;                  // ODE function type taking (double, dVec) and returning dVec
using DelayFunc      = std::function<dVec(double, const dVec&, const dVec&)>;     // DDE function type (single delay) taking (double, dVec, dVec) and returning dVec
using DelayFuncMulti = std::function<dVec(double, const dVec&, const dMatrix&)>;   // DDE function type (multiple delays) taking (double, dVec, dMatrix) and returning dVec
```

- **`Vec<T>`** - A template alias for `std::vector<T>`, allowing for easier declaration of vectors with different data types.
- **`dVec`** - An alias for a vector of doubles (`std::vector<double>`), commonly used for numerical computations.
- **`wVec`** - An alias for a vector of `size_t` integers, often used for indexing and sizes.
- **`dcVec`** - An alias for a vector of complex doubles (`std::vector<std::complex<double>>`), useful for computations involving complex numbers.
- **`iVec`** - An alias for a vector of integers (`std::vector<int>`), used for integer data.
- **`uVec`** - An alias for a vector of unsigned integers (`std::vector<unsigned int>`), used when negative values are not needed.
- **`bVec`** - An alias for a vector of booleans (`std::vector<bool>`), useful for binary data.
- **`sVec`** - An alias for a vector of strings (`std::vector<std::string>`), used for text data.
- **`dpairVec`** - An alias for a vector of pairs of doubles (`std::vector<std::pair<double, double>>`), useful for storing coordinate pairs or related double values.
- **`spairVec`** - An alias for a vector of pairs of strings (`std::vector<std::pair<std::string, std::string>>`), useful for storing related string pairs.
- **`triple`** - An alias for a fixed-size array of three doubles (`std::array<double, 3>`), useful for representing 3D coordinates or vectors.
- **`dMatrix`** - An alias for a 2D matrix represented as a vector of vectors of doubles (`std::vector<std::vector<double>>`), commonly used for matrix operations.
- **`func`** - An alias for a function type that takes a double as input and returns a double (`std::function<double(double)>`), useful for defining mathematical functions.
- **`Func`** - An alias for a function type that takes a double as input and returns a vector of doubles (`std::function<dVec(double)>`), useful for defining functions that return multiple values (i.e. a function that actually represents a set of normal functions--`func`--as in a system of equations).
- **`MyFunc`** - An alias for a function type that takes a double and a vector of doubles as input and returns a vector of doubles (`std::function<dVec(double, const dVec&)>`), specifically designed for defining ordinary differential equation (ODE) systems.
- **`DelayFunc`** - An alias for a function type that takes a double and two vectors of doubles as input and returns a vector of doubles (`std::function<dVec(double, const dVec&, const dVec&)>`), specifically designed for defining delay differential equation (DDE) systems with a single delay.
- **`DelayFuncMulti`** - An alias for a function type that takes a double, a vector of doubles, and a matrix of doubles as input and returns a vector of doubles (`std::function<dVec(double, const dVec&, const dMatrix&)>`), specifically designed for defining delay differential equation (DDE) systems with multiple delays.

### `struct`s For Data-Structure

There are multiple data-structures that have been used in this library, but some of them are rather *central* to the project. The idea is to have them defined in the main `header.hpp` so that every other component knows about it without any `#include` requirement (the minor data-structures are defined locally whereever they are needed!).

- **Solver Parameters** - The centralized definition for parameters to be fed to a solver is as follows:

    ```C++
    struct SolverParameters 
    {
        MyFunc derivative;                           // ODE function
        dVec   initial_conditions;
        double t0                    = 0.0;         // Start time
        double t1                    = 10.0;        // End time
        double dt                    = 1.0 / 16.0;  // Initial step size
        int    order                 = 4;           // Order for AB/ABM
        int    iterations            = 2;           // Iterations for ABM
        double min_dt                = 1.0 / 128.0; // Minimum step size (for variable step size control)
        double max_dt                = 0.5;         // Maximum step size (for variable step size control)
        double decrease_factor       = 0.9;
        double increase_factor       = 2.0;
        double local_tol             = 1.0e-7;      // Local error tolerance
        double absolute_tol          = 1.0e-10;     // Takes care of division by zero (NaN and/or inf values)
        double local_tol_error_ratio = 0.1;  
        size_t max_trial             = 25;
        bool   error_estimate        = false;      // Wether to use error estimation (or not)
        bool   variable_steps        = false;      // Wether to use variable step size (or not)
        bool   attempts_history      = false;      // Controls wether the errors and step sizes are stored for all attempts or not.
        bool   weighted_error        = true;       // Calculate error based on weighted error formula
        bool   norm_error            = false;      // Calculate error based on norm error formula (norm or the solution is used instead of maximum of the absolute value of the elements of the solution)
        // DDE-specific parameters
        DelayFunc delay_derivative   = nullptr;     // DDE function f(t, y, y_delayed)
        DelayFuncMulti delay_derivative_multi = nullptr; // DDE function f(t, y, y_delayed_states) for multiple delays
        dVec      delay_times;                      // Vector of delay times τ_i
        Func      history_function;                 // Initial history function y(t) for t < t0
        bool      is_dde            = false;        // Flag to indicate DDE vs ODE
        size_t    max_delay_order   = 10;           // Maximum order for delay terms
        double    interpolation_tol = 1.0e-6;       // Tolerance for delay interpolation
        double    dt_scale          = 1.0e-1;  // tau_min is compared to dt_scale*dt to determine a fine time step.
    };
    ```

    1. **`MyFunc derivative`** - The function representing the system of ordinary differential equations (ODEs) to be solved (RHS of the ODE).
    2. **`dVec initial_conditions`** - A vector containing the initial conditions for the ODE system.
    3. **`double t0`** - The start time for the simulation (default is 0.0).
    4. **`double t1`** - The end time for the simulation (default is 10.0).
    5. **`double dt`** - The initial time step size for the solver (default is 1/16, it is constant unless an adaptive solver is used).
    6. **`int order`** - The order of the numerical method to be used (default is 4, applicable for multistep methods like Adams-Bashforth-Moulton).
    7. **`int iterations`** - The number of iterations for the Adams-Bashforth-Moulton method, i.e. the correction step (default is 2).
    8. **`double min_dt`** - The minimum allowable time step size for adaptive solvers (default is 1/128).
    9. **`double max_dt`** - The maximum allowable time step size for adaptive solvers (default is 0.5).
    10. **`double decrease_factor`** - The factor by proportion to which to decrease the time step size when the error exceeds the tolerance (default is 0.9).
    11. **`double increase_factor`** - The factor by proportion to which to increase the time step size when the error is within the tolerance (default is 2.0).
    12. **`double local_tol`** - The local error tolerance for adaptive solvers (default is 1.0e-7).
    13. **`double absolute_tol`** - The absolute tolerance to prevent division by zero in error calculations (default is 1.0e-10).
    14. **`double local_tol_error_ratio`** - A ratio used in error estimation for adaptive solvers (default is 0.1).
    15. **`size_t max_trial`** - The maximum number of trials for adjusting the time step size in adaptive solvers (default is 25).
    16. **`bool error_estimate`** - A flag indicating whether to use error estimation in the solver (default is false).
    17. **`bool variable_steps`** - A flag indicating whether to use variable step sizes in the solver (default is false).
    18. **`bool attempts_history`** - A flag to control whether to store errors and step sizes for all attempts (default is false).
    19. **`bool weighted_error`** - A flag to indicate whether to calculate error based on a weighted error formula (default is true).
    20. **`bool norm_error`** - A flag to indicate whether to calculate error based on the norm of the solution (default is false).
    21. **`DelayFunc delay_derivative`** - The function representing the system of delay differential equations (DDEs) for single delay (default is nullptr).
    22. **`DelayFuncMulti delay_derivative_multi`** - The function representing the system of delay differential equations (DDEs) for multiple delays (default is nullptr).
    23. **`dVec delay_times`** - A vector containing the delay times $τ_i$ for the DDE system.
    24. **`Func history_function`** - The history function $y(t)$ for times $t < t0$, used to define initial conditions for DDEs.
    25. **`bool is_dde`** - A flag indicating whether the problem is a DDE (true) or an ODE (false) (default is false).
    26. **`size_t max_delay_order`** - The maximum order for delay terms in the DDE solver (default is 10).
    27. **`double interpolation_tol`** - The tolerance for interpolation in delay terms (default is 1.0e-6).
    28. **`double dt_scale`** - A scaling factor used to determine a fine time step based on the minimum delay time (default is 1.0e-1). The *fine* time step is determined according to these lines in solvers provided within [`adams-solvers-dde.hpp`](includes/solvers/DDE/multistep/adams-solvers-dde.hpp) file

        ```C++
            // Initialize solution storage
            const size_t num_per_steps = std::max(static_cast<size_t>(8), static_cast<size_t>(std::ceil(1.0 / user_dt)));
            const double min_dt        = tau_min / num_per_steps;
            const double dt            = (tau_min < Params.dt_scale * user_dt) ? tau_min : std::min(user_dt, min_dt);
        ```

- **Solution Structure** - The centralized definition for the solution returned by a solver is as follows:

    ```C++
    struct SolverResults 
    {
        dMatrix      solution;       // The solution matrix.
        dVec        time_points;    // Time points where solution is computed.
        dVec        errors;         // Error estimates.
        dVec        step_sizes;     // Actual step sizes used.
        dMatrix      steps_history;  // History of step sizes used (contains failed steps attempted).
        dMatrix      errors_history; // History of errors (contains failed steps attempted).
        wVec failed_trials;  // Number of failed trials in each step.
    };
    ```

    1. **`dMatrix solution`** - A matrix where each row corresponds to the state of the system at a specific time point.
    2. **`dVec time_points`** - A vector containing the time points at which the solution is computed.
    3. **`dVec errors`** - A vector containing error estimates for each time step.
    4. **`dVec step_sizes`** - A vector containing the actual step sizes used during the simulation (Solving the differential equation).
    5. **`dMatrix steps_history`** - A matrix containing the history of step sizes used, including those from failed attempts (if `attempts_history` is enabled).
    6. **`dMatrix errors_history`** - A matrix containing the history of errors, including those from failed attempts (if `attempts_history` is enabled).
    7. **`wVec failed_trials`** - A vector containing the number of failed trials for each time step.

- **Sparse Vector** - A simple structure to represent sparse vectors:

    ```C++
    struct SparsedVec  
    {
        std::vector<std::pair<size_t, double>> values; // (index, value) pairs
        SparsedVec(size_t N) : values(N) {}
        void add(size_t i, double value) { values.emplace_back(i, value); }
    };
    ```

    1. **`std::vector<std::pair<size_t, double>> values`** - A vector of pairs where each pair consists of an index and its corresponding non-zero value in the sparse vector.
    2. **`SparsedVec(size_t N)`** - A constructor that initializes the sparse vector with a specified size `N` (size of non-zero vector).
    3. **`void add(size_t i, double value)`** - A method to add a new (index, value) pair to the sparse vector.

- **Sparse dMatrix** - A simple structure to represent sparse matrices:

    ```C++
    struct SparsedMat  
    {
        std::vector<SparsedVec> rows; // Vector of sparse vectors representing rows
        SparsedMat(size_t N) : rows(N) {}
        void add(size_t row, size_t col, double value) { rows[row].add(col, value); }
    };
    ```

    1. **`std::vector<SparsedVec> rows`** - A vector of `SparsedVec` objects, each representing a row of the sparse matrix.
    2. **`SparsedMat(size_t N)`** - A constructor that initializes the sparse matrix with `N` rows.
    3. **`void add(size_t row, size_t col, double value)`** - A method to add a new (column index, value) pair to a specific row in the sparse matrix.

You may add any `struct` (OR `class`) that has global use-case (i.e. other components might need them) here next to the ones discussed above (and please, add the corresponding documentations).

## The Library (`includes`)

The library is something like this

```text
includes/
├─ initializers/
│  └─ initials.hpp
├─ interpolators/
│  ├─ lagrange-interpolator.hpp
│  └─ newton-interpolator.hpp
├─ linalg/
│  ├─ basics/
│  │  ├─ decomposers.hpp
│  │  ├─ eigen.hpp
│  │  ├─ matrix-operations.hpp
│  │  └─ matrix-properties.hpp
│  └─ linsyssolvers/
│     ├─ banded-solver.hpp
│     ├─ gaussian-elimination.hpp
│     ├─ ldlt-solve.hpp
│     ├─ llt-solve.hpp
│     ├─ lu-solve.hpp
│     ├─ qr-solve.hpp
│     ├─ svd-solve.hpp
│     └─ tridiagonal-solver.hpp
├─ miscellany/
│  ├─ ABM-Coefs/
│  │  ├─ abm_coefs_order1.txt
│  │  ├─ abm_coefs_order2.txt
│  │  ├─ abm_coefs_order3.txt
│  │  ├─ abm_coefs_order4.txt
│  │  ├─ abm_coefs_order5.txt
│  │  ├─ abm_coefs_order6.txt
│  │  ├─ abm_coefs_order7.txt
│  │  ├─ abm_coefs_order8.txt
│  │  ├─ abm_coefs_order9.txt
│  │  └─ abm_coefs_order10.txt
│  ├─ abm-v0.0.py
│  └─ abm.py
├─ models/
│  ├─ kuramoto/
│  │  ├─ general.hpp
│  │  ├─ order.hpp
│  │  ├─ sparse.hpp
│  │  └─ special.hpp
│  ├─ models.phase-oscillators.hpp
│  ├─ kuramoto.hpp
│  ├─ OA.hpp
│  └─ ott-antonsen.dim-red/
│     ├─ OA-basic.hpp
│     ├─ OA-expanded.hpp
│     ├─ OA-helper.hpp
│     ├─ OA-mh.md
│     ├─ OA.html
│     └─ OA.md
├─ namespaces/
│  └─ abm-coefs.hpp
├─ network/
│  └─ topology.hpp
├─ solvers/
│  ├─ DDE/
│  │  ├─ adaptive/
│  │  ├─ multistep/
│  │  │  └─ adams-solvers-dde.hpp
│  │  └─ rk/
│  │     └─ explicit/
│  ├─ miscellany/
│  │  └─ validator.hpp
│  └─ ODE/
│     ├─ adaptive/
│     │  ├─ adaptive-common.hpp
│     │  └─ adaptive-custom.hpp
│     ├─ multistep/
│     │  ├─ ab-solver.hpp
│     │  ├─ ab-split.hpp
│     │  └─ abm-solver.hpp
│     └─ rk/
│        └─ explicit/
│           ├─ rk1-solver.hpp
│           ├─ rk2-solver.hpp
│           ├─ rk3-solver.hpp
│           ├─ rk4-solver.hpp
│           └─ rk4-variants.hpp
├─ typedefs/
│  ├─ complex.hpp
│  └─ header.hpp
├─ utility/
│  ├─ adaptive-solvers-utility.hpp
│  ├─ print.hpp
│  ├─ utility.hpp
│  └─ write.hpp
└─ mm.hpp
```

and the whole project tree is:

```text
math-mod-tk/
├─ .github/
├─ .vscode/
├─ docs/
├─ Examples/
├─ includes/
└─ LICENSE
```

### Initializers

This directory contains headers for initializing system (simulation) state and parameters required, e.g. intrinsic phases of the phase oscillators (kuramoto model), initial state (i.e. initial phases for phase oscillators), etc. You may use it and/or add new functions or headers depending the *model* that you wish to simulate/analyse.

**Example function:** **`random_uniform`** - This function initializes the phases of oscillators randomly with a uniform distribution between 0 (`min_val`) and 2π (`max_val`).

```C++
inline dVec  random_uniform(
    size_t   N,
    double   min_val,
    double   max_val,
    unsigned seed
)
{
    // Error Handling
    if (N == 0) 
    {
        throw std::invalid_argument("[random_uniform_phases] Number of phases N cannot be zero.");
    }
    if (min_val > max_val) 
    {
        throw std::invalid_argument("[random_uniform_phases] Min value (" + std::to_string(min_val) + ") cannot be greater than max value (" + std::to_string(max_val) + ").");
    }

    // Original logic
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> dist(min_val, max_val);
    dVec  phases(N);
    for (auto& x : phases)
        x = dist(rng);
    return phases;
}
```

**Usage Example:** You may use this within you main simulation code (i.e. `main.cpp`, `simulation.cpp`, etc.) as follows (this assumes the file is located at `includes/initializers/initials.hpp` and the source file at the same directory level as `includes`):

```C++
#include "includes/initializers/initials.hpp"

int main()
{
    dVec phases = random_uniform(100, 0.0, 2 * PI, 123); // 100 oscillators with phases between 0 and 2π, the seed for the uniform_real_distribution is set to 123
}
```

For further details, please refer to the comments within each header file in the `initializers` directory. And for a run-down on the functions of initializers, please refer to the documentation ([initials.hpp documentation](./docs/includes.initializers/initials.md)).

### interpolators

This directory contains headers that provide function interpolation capabilities. These are particularly useful in the context of Delay Differential Equations (DDEs), where the solver may need to estimate the value of a function at points that are not explicitly computed during the simulation.

**Example function:** **`lagrange_interpolator`** - This function takes an array of `x` values and their corresponding `y` values, and returns a lambda function that performs Lagrange interpolation for a **single-valued** scalar (real) function *$y=f(x)\approx P(x), y_{i} = P(x_{i}) = f(x_{i})$*.

```C++
inline func lagrange_interpolator(const dVec& x_points, const dVec& y_points)
{
    return [x_points, y_points](double x) -> double {
        double result = 0.0;
        size_t n = x_points.size();
        for (size_t i = 0; i < n; ++i)
        {
            double term = y_points[i];
            for (size_t j = 0; j < n; ++j)
            {
                if (i != j)
                    term *= (x - x_points[j]) / (x_points[i] - x_points[j]);
            }
            result += term;
        }
        return result;
    };
}
```

**Usage Example:** You may use this within you main simulation code (i.e. `main.cpp`, `simulation.cpp`, etc.) as follows (this assumes the file is located at `includes/interpolators/lagrange-interpolator.hpp` and the source file at the same directory level as `includes`):

```C++
#include "includes/interpolators/lagrange-interpolator.hpp"

int main()
{
    dVec x_points = {0.0, 1.0, 2.0}; // A vector of x-coordinates
    dVec y_points = {1.0, 2.0, 0.0}; // Corresponding y-coordinates f(x_i)
    func interpolator = lagrange_interpolator(x_points, y_points);
    double x = 1.5; // Estimate the value of f(x) at x = 1.5
    double interpolated_value = interpolator(x); // Estimated value is written to interpolated_value variable.
}
```

For further details, please refer to the comments within each header file in the `interpolators` directory. And for a run-down on the functions of interpolators, please refer to the documentation ([`lagrange-interpolator.hpp documentations](docs/includes.interpolators/lagrange-interpolator.md), and ['newton-interpolator.hpp documentations](docs/includes.interpolators/newton-interpolator.md)).

### linalg

This directory is meant to be a collection of useful linear algebra utilities, while keeping dependencies to a minimum. It includes basic matrix operations, properties, and various linear system solvers.

For further details, please refer to the comments within each header file in the `linalg` directory. And for a run-down on the functions of linalg, please refer to the documentation:

- [**Main linalg Documentation**](./docs/includes.linalg/linalg.md) - Overview, quick start, and module structure
- [**Basics Module**](./docs/includes.linalg/basics/basics.md) - dMatrix operations, properties, and decompositions
- [**Solvers Module**](./docs/includes.linalg/solvers/solvers.md) - Linear system solvers and selection guide

### miscellany

This directory is a one-time use collection of files it has been written for the specific purpose of serving as a directory for pre-processed data files and scripts that produce the data that is being used in the actual library implementation. At the time being there is only one script that uses `sympy` to generate coefficients for the *Adams-Bashforth-Moulton* (ABM) and *Adams-Bashforth* (AB) multistep ODE solvers. These coefficients are stored as text files in the `ABM-Coefs` sub-directory.

The solvers are particularly implemented in the `solvers/ODE/multistep/` directory. And also modified versions have been implemented for DDEs as well (in the `solvers/DDE/multistep/` directory). See [the documentation](docs/includes.miscellany/miscellany.md) for further details.

### models

This directory contains implementations of the **actual** mathematical models and simulations that the library is focused on. Currently, it includes implementations for the Kuramoto model of coupled phase oscillators and its dimensionality reduction using the Ott-Antonsen ansatz. The `User` may add their own models to this directory, for example, the *FitzHugh-Nagumo* model, *Hodgkin-Huxley* model, etc.

As a cautionary, please ensure that any new models added to this directory are compatible with the existing solvers and utilities in the library. This may require additional implementation or modification of existing code.

Demonstrative example could be:

```C++
// General Kuramoto model with phase-lag and flexible dMatrix
inline dVec  kuramoto_general(
    double              time,
    const dVec&         theta,
    const dVec&         omega,
    double              K,
    const dMatrix&       adj,
    double              alpha
)
{
    const size_t N = theta.size(); // N is derived from theta, so N > 0 here.

    // Original logic
    dVec  dtheta_dt = dVec( N, 0.0 );
    auto  k_norm    = K / static_cast<double>( N ); // Renamed k to k_norm for clarity
    for ( size_t i = 0; i < N; ++i )
    {
        double sum = 0.0;
        for ( size_t j = 0; j < N; ++j )
        {
            sum += adj[i][j] * std::sin( theta[j] - theta[i] - alpha );
        }
        dtheta_dt[i] = omega[i] + k_norm * sum;
    }
    return dtheta_dt;
}
```

This function takes the argument list of `time`, `theta` (phases), `omega` (intrinsic frequencies), `K` (coupling strength), `adj` (adjacency matrix), and `alpha` (phase lag) to compute the time derivative of the phases according to the generalized Kuramoto model. It returns a vector of phase derivatives `dVec dtheta_dt`.

There is a catch with leaving the model definition at this. The *solvers* need a function of this form `dVec f(doule, const dVec&)` to work with. Therefore, wrapper functions are needed to adapt the model functions to the required signature. These wrappers can be defined in the `models` directory as well. For example:

```C++
struct KuramotoParams
{
    // Model specific parameters in Kuramoto model (time and phases/theta are dynamics bounds)
    double K;     // Coupling strength
    int    N;     // Number of oscillators
    dVec   omega; // Natural frequencies
    dMatrix adj;   
    double alpha;

    KuramotoParams(int num_oscillators, double phae_lag = 0.0)
        : N(num_oscillators),
          omega(dVec(num_oscillators, 0.0)),
          adj(dMatrix(num_oscillators, dVec(num_oscillators, 0.0))),
          alpha(phae_lag)
    {}
};

inline MyFunc kuramoto_general_wrapper(const KuramotoParams& params)
{
    return [params](double t, const dVec& theta) -> dVec
    {
        return kuramoto_general(t, theta, params.omega, params.K, params.adj, params.alpha);
    };
}
```

You may now use this as follows:

```C++
#include "includes/models/kuramoto/general.hpp"
#include "includes/solvers/ODE/rk/explicit/rk4-solver.hpp"

int main()
{
    // Define model parameters
    KuramotoParams params(100); // 100 oscillators
    params.K = 1.0; // Set coupling strength
    // Initialize other parameters as needed... params.omega, params.adj, etc. Note that the initializers/initials.hpp file may help here.

    // Create the model function using the wrapper
    MyFunc kuramoto_model = kuramoto_general_wrapper(params);

    // Define initial conditions
    dVec initial_phases = random_uniform(params.N, 0.0, 2 * PI, 42);

    // Time span for the simulation
    SolverParameters rk4_params;
    rk4_params.dt = 0.01; // Time step
    rk4_params.derivative = kuramoto_model;
    // Proceed to set other values of rk4_params as needed... t0, t1, etc.

    // Solve the ODE
    auto sol = rk4_solver(rk4_params);
    auto phases = sol.solution;
    dVec final_phases = phases.pop_back();
    // final_phases now contains the phases of the oscillators at time tf
    // You may now process or analyze final_phases as needed.
}
```

For further details, please refer to the comments within each header file in the `models` directory. And for a run-down on the functions of models, please refer to the documentation [models.md](docs/includes.models/models.md).

#### Notes On Handling Models

**Note:** Similar wrapper functions can be created for other models to ensure compatibility with the solver interfaces. For all the functions within the `models` directory, such wrappers are provided.

**Note:** The `models` has three main header files, namely `models.phase-oscillators.hpp`, `kuramoto.hpp`, and `OA.hpp`. These files include the necessary model definitions and wrappers for phase oscillator models, the Kuramoto model, and the Ott-Antonsen dimensionality reduction, respectively. As a measure of consistency and ease of use, it is advisale to follow these steps:

1. Define the model functions and headers inside a separate subdirectory (e.g. `models/stochastic`, etc.) and populate the subdirectory with the necessary model implementations.
2. Create wrapper functions for each model to adapt them for easier use/re-use with the solvers, e.g. `dVec lotka_voltera(const lv_params& params)` and `MyFunc lotka_voltera_wrapper(const lv_params& params)`.
3. Include the model implementations and wrappers in the main model header files (`kuramoto.hpp`, `OA.hpp`, etc.) for easy access.
4. If the model has multiple subparts (i.e. `kuramot`, and `ott-antonsen.dim-red`) it is better to have them in a centralised header (i.e. `model.phase-oscillators.hpp`) for easier inclusion in the main model headers.
5. Document each model and its parameters thoroughly within the header files to ensure clarity for future users and maintainers of the library. Also add proper documentations to the `docs` directory. (Please follow the same style and aesthetic while adding/contributing to the project.)

### namespaces

This directory contains headers that define and store repeatedly used constants and coefficients for various numerical methods implemented in the library. This includes coefficients for multistep methods like Adams-Bashforth-Moulton (ABM) and Adams-Bashforth (AB) methods. They all reside within the `abm_coefs` namespace. The `User` may add more namespace(s) as they see fit for their own implementations. Please note that when a set of functions use a set of numbers repeatedly, it is better to store them in a namespace for easier access and maintainability. (Also the code would be clearer and there would be less clutter.)

For the provided namespace(s), there are the following documentation(s):

1. [ABM Coefficients](docs/includes.namespaces/abm-coefs.md)

Feel free to add your own documentations, after you implemented new namespace(s).

### network

This directory contains headers related to network topology and graph structures. It includes functions for creating, manipulating, and analyzing network topologies, which are essential for simulating systems of coupled oscillators or other network-based models.

#### Function List

1. [`topology.hpp`](docs/includes.network/topology.md) - Functions for generating and handling network topologies.
    1. **dMatrix conversion & analysis**
        - `dense_to_sparse(const dMatrix&)` — Convert dense matrix to sparse format
        - `density(const dMatrix&, double)` — Compute sparsity density (fraction of nonzero off-diagonal entries)
        - `dense_to_sparse_conditional(const dMatrix&, SparsedMatrix&, double, double)` — Conditionally convert to sparse if below density threshold
        - `in_degrees(const dMatrix&, double)` / `in_degrees(const SparsedMatrix&, double)` — Count incoming edges per node
        - `out_degrees(const dMatrix&, double)` / `out_degrees(const SparsedMatrix&, double)` — Count outgoing edges per node
    2. **Basic topology generators**
        - `random(size_t, double, double, unsigned)` — Generate random network with uniform edge weights
        - `random_symmetric(size_t, double, double, unsigned)` — Generate symmetric random network
    3. **Erdős–Rényi networks**
        - `erdos_renyi(size_t, double, double, double, unsigned)` — Random graph with connection probability $p$
        - `erdos_renyi_uniform(size_t, double, double, double, unsigned)` — Erdős–Rényi with exactly $N(N-1)p$ edges
        - `erdos_renyi_symmetric(size_t, double, double, double, unsigned)` — Symmetric Erdős–Rényi
        - `erdos_renyi_symmetric_uniform(size_t, double, double, double, unsigned)` — Symmetric with exactly $N(N-1)p/2$ edges
    4. **Small-world networks (Watts–Strogatz)**
        - `small_world(size_t, size_t, double, double, unsigned)` — Ring lattice with rewiring (undirected)
        - `small_world_sparse(size_t, size_t, double, double, unsigned)` — Small-world in sparse format
        - `small_world_directed(size_t, size_t, double, double, unsigned)` — Directed small-world network
    5. **Structured networks**
        - `multilayered(const std::vector<dMatrix>&)` — Block-diagonal composition of layer matrices
        - `modular(size_t, size_t, double, double, double, double, unsigned)` — Modular network with dense within-module and sparse between-module connections
        - `hierarchical(size_t, size_t, double, double, double, double, double, unsigned, size_t)` — Recursively nested modules with level-dependent connection probabilities
    6. **Multiplex networks**
        - `effective_multiplex(const std::vector<dMatrix>&, const dVec&)` — Weighted sum of multiple network layers
        - `generate_multiplex_network_layers(size_t, size_t, const std::vector<std::string>&, const dMatrix&, const std::vector<unsigned>&)` — Generate multiple layers with specified topologies
    7. **Unified interface**
        - `pick_topology(const std::string&, size_t, double, double, double, unsigned, double, double, double, size_t)` — Create network by name with parameter dispatch.

For further details, please refer to the comments within each header file in the `network` directory. And for a run-down on the functions of network, please refer to the documentation [topology.md](docs/includes.network/topology.md).

### solvers

The solvers are the core components of the library for solving the mathematical formulae governing the systems. This module is divided into multiple parts.

1. **ODE solvers:** These are the solvers for the systems governed by *Ordinary Differential Equations*. This is also divided into parts:
    1. **Runge-Kutta family of solvers:** These include explicit forms of RK solvers of order $1$ to $4$ (RK1 to RK4). They are implemented in the `solvers/ODE/rk/explicit/` directory. And also there are aliases for functions (`rk1` is the same as `euler`, etc.) and there are variants of rk4 solver including $\frac{3}{8}$ `rule`, etc.
    2. **Multi-step solvers:** These are the solvers that use multiple previous steps to compute the next step. The Adams-Bashforth (AB) and Adams-Bashforth-Moulton (ABM) solvers are implemented here. They are located in the `solvers/ODE/multistep/` directory.
    3. **Adaptive solvers:** These solvers adjust their step size based on error estimates to achieve a desired accuracy. They are implemented in the `solvers/ODE/adaptive/` directory.
2. **DDE solvers:** These are mostly the same as the **ODE solvers** with some adaptations and modifications to be able to integrate *Delay Differential Equations*. As of now the supported types of *DDEs* are:
    1. **Single Constant Delay:**

        $$ \displaystyle \dot{y}(t) = f\left(t; y\left(t\right), y\left(t-\tau\right)\right). $$

    2. **Multiple Constant Delays:**

        $$ \displaystyle \dot{y}(t) = f\left(t; y\left(t\right), y\left(t-\tau_1\right), y\left(t-\tau_2\right), \ldots, y\left(t-\tau_m\right)\right). $$

**Documentations** - See below for documentations of specific solvers/headers.

1. **ODE solvers**
    1. [rk1 solver](docs/includes.solvers/ODE/rk/rk1.md)
    2. [rk2 solver](docs/includes.solvers/ODE/rk/rk2.md)
    3. [rk3 solver](docs/includes.solvers/ODE/rk/rk3.md)
    4. [rk4 solver](docs/includes.solvers/ODE/rk/rk4.md)
    5. [rk4 variants](docs/includes.solvers/ODE/rk/rk4-variants.md)
    6. [multi-step solvers](docs/includes.solers/ODE/multistep/multi-step.md)
    7. [adaptive solvers](docs/includes.solvers/ODE/adaptive/adaptive-solvers.md)
2. **DDE solvers**
    1. [constant delay multistep solvers](docs/includes.solvers/DDE/DDE-constant-delay.md)

### typedefs

All *types*, *aliases*, or important declarations and *global* variable are meant to be done within this folder, [`header.hpp`](includes/typedefs/header.hpp) is the main *header file* for this project. Also all `#include`s that are necessary for general usage are provided there (if you need a specific `#include` that has no usage outside of the module you defined it is better to keep that inside the module, otherwise add it to the `header.hpp` file). The `complex.hpp` file contains the definition for complex numbers and related operations (it is not a serious part of the project yet). You may add your own type definitions, aliases, or global variables to these files as needed.

For the details about the constents of the `header.hpp` see [core practices here](#core-practices) and [core definitions and notations here](#core-definitions-and-notations)

### utility

This directory is for providing functions that either are for general utility (i.e.`write.hpp`), or for a specific module/set of modules that would need repitition if defined per each file (i.e. `adaptive-solvers-utility.hpp`).

There are the following files in this directory:

1. [`adaptive-solvers-utility.hpp`](docs/includes.utility/adaptive-solvers-utility.md)
2. [`print.hpp`](docs/includes.utility/print.hpp)
3. [`utility.hpp`](docs/)
4. [`write.hpp`](docs/)
5. `utility.hpp` is meant to be a collection of all utilities, it is still better to `#include` the required header individually.

If you need to add any function that matches the description above, please add it to this directory and document it properly. Also add the corresponding documentation to the `docs` directory. (Please follow the same style and aesthetic while adding/contributing to the project.)

## Compilation Notes

Instances of compilation of the codes would be:

```bash
g++ -O3 -std=c++23 code.cpp -o executable
./executable
```

OR

```bash
g++ -O3 -pthread -std=c++23 -funroll-loops -ffast-math -static -flto code.cpp -o executable
```
