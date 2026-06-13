# Adaptive ODE Solvers

Adaptive ODE solvers automatically adjust the step size during integration to balance accuracy and efficiency. This document describes the adaptive Runge–Kutta methods implemented in the library: Heun–Euler (RK1(2)) and RK1–RK2 solvers with embedded error estimation and step-size control.

## Table of Contents

1. [Adaptive ODE Solvers](#adaptive-ode-solvers)
    1. [Table of Contents](#table-of-contents)
    2. [Overview](#overview)
    3. [Adaptive Step-Size Control](#adaptive-step-size-control)
        1. [Error Estimation](#error-estimation)
        2. [Step-Size Adjustment](#step-size-adjustment)
    4. [Heun–Euler Method (RK1(2))](#heuneuler-method-rk12)
        1. [Derivation and Algorithm](#derivation-and-algorithm)
        2. [Order and Convergence](#order-and-convergence)
    5. [RK1–RK2 Custom Method](#rk1rk2-custom-method)
        1. [Structure and Efficiency](#structure-and-efficiency)
    6. [Implementation and Usage](#implementation-and-usage)
        1. [Example: Adaptive Heun–Euler Solver](#example-adaptive-heuneuler-solver)
        2. [Example: Adaptive RK1–RK2 Custom Solver](#example-adaptive-rk1rk2-custom-solver)
    7. [Configuration and Parameters](#configuration-and-parameters)
    8. [Practical Remarks](#practical-remarks)
    9. [Notes](#notes)
    10. [Other solvers](#other-solvers)
         1. [Standard Embedded Pairs (adaptive-common.hpp)](#standard-embedded-pairs-adaptive-commonhpp)
             1. [Bogacki–Shampine (RK2(3))](#bogackishampine-rk23)
             2. [Fehlberg (RK3(4))](#fehlberg-rk34)
             3. [Dormand–Prince (RK4(5))](#dormandprince-rk45)
         2. [Custom Embedded Pairs (adaptive-custom.hpp)](#custom-embedded-pairs-adaptive-customhpp)
             1. [RK2–RK3 Custom](#rk2rk3-custom)
             2. [RK3–RK4 Custom](#rk3rk4-custom)
             3. [RK4–RK5 Custom](#rk4rk5-custom)
         3. [Usage](#usage)
         4. [Other methods (not implemented yet)](#other-methods-not-implemented-yet)

## Overview

**Adaptive solvers** solve ODEs of the form

$$\displaystyle y' = f(t,y), \quad y(t_0) = y_0$$

using an **embedded pair** of methods with different orders to estimate local truncation error and adjust step size automatically. This avoids both excessive refinement in smooth regions and failure to resolve steep changes, typically outperforming fixed-step methods on problems with varying stiffness or regularity.

The library provides two adaptive pairs:

- **Heun–Euler (RK1(2))**: pairs RK1 (Euler, order 1) with Heun (order 2); simple and robust.
- **RK1–RK2 Custom**: pairs RK1 with modified RK2 (midpoint); more efficient slope reuse.

Both support three modes:

1. **No error estimation**: plain fixed-step integration (baseline).
2. **Error estimation (fixed step)**: compute error per step for diagnostics, keep $dt$ constant.
3. **Adaptive stepping**: vary $dt$ to keep per-step error within tolerance $\varepsilon$.

## Adaptive Step-Size Control

### Error Estimation

For each step, an embedded error pair computes two approximations of $y_{n+1}$:

- **Lower-order method** (RK1 or similar): denoted $\tilde{y}_n$.
- **Higher-order method** (Heun, RK2): denoted $\hat{y}_n$.

The **local error estimate** is

$$\displaystyle e_n \approx \| \hat{y}_n - \tilde{y}_n \|,$$

which approximates the true local truncation error modulo problem conditioning.

### Step-Size Adjustment

After computing error $e_n$, the controller adjusts the step size if the error exceeds or falls below target bounds. Let $\varepsilon$ be the target tolerance per step. A simple proportional controller sets the next step to

$$\displaystyle h_{\text{new}} = h_{\text{old}} \left(\frac{\varepsilon}{e_n}\right)^{1/(p+1)},$$

where $p$ is the order of the higher-order method. The exponent $1/(p+1)$ ensures stability of the feedback loop. In practice:

- If $e_n > \varepsilon$: **reject** the step, reduce $h$, and retry.
- If $e_n \leq \varepsilon$: **accept** the step and increase $h$ slightly for efficiency.

A **maximum number of trials** prevents infinite loops if the problem is not solvable at small enough $dt$.

## Heun–Euler Method (RK1(2))

The Heun–Euler pair combines:

- **Euler** (RK1, order 1): $\tilde{y}_{n+1} = y_n + h \, f(t_n, y_n)$.
- **Heun** (order 2): $\hat{y}_{n+1} = y_n + \frac{h}{2} (f(t_n, y_n) + f(t_n+h, \tilde{y}_{n+1}))$.

### Derivation and Algorithm

**Euler's method** (as reviewed in `rk1.md`) evaluates the slope once at $t_n$:

$$\displaystyle \tilde{y}_{n+1} = y_n + h \, k_1, \quad k_1 = f(t_n, y_n).$$

**Heun's method** (a two-stage explicit RK method) re-uses $k_1$ and adds a half-weighted average with the slope at the end:

$$\displaystyle k_1 = f(t_n, y_n),$$

$$\displaystyle k_2 = f(t_n + h, y_n + h \, k_1),$$

$$\displaystyle \hat{y}_{n+1} = y_n + \frac{h}{2}(k_1 + k_2).$$

The error difference provides an error estimate:

$$\displaystyle e_n = \| \hat{y}_{n+1} - \tilde{y}_{n+1} \| = \left\| \frac{h}{2}(k_1 + k_2) - (y_n + h \, k_1) \right\| = \frac{h}{2} \| k_2 - k_1 \|.$$

Algorithm:

1. Compute $k_1 = f(t_n, y_n)$.
2. Compute $k_2 = f(t_n + h, y_n + h \, k_1)$ (Euler predictor).
3. Update with Heun: $\hat{y}_{n+1} = y_n + \frac{h}{2}(k_1 + k_2)$ (accepted solution).
4. Estimated error: $e_n \approx \frac{h}{2}\|k_2 - k_1\|$ (or component-wise norm).
5. If adaptive: adjust $h$ and retry if $e_n > \varepsilon$; otherwise accept step.

### Order and Convergence

- **Local error (Heun)**: $\mathcal{O}(h^3)$ (order 2, second-order accurate).
- **Global error**: $\mathcal{O}(h^2)$ over $t \in [t_0, t_1]$.
- **Efficiency gain vs. fixed RK2**: adaptive stepping reduces $h$ in smooth regions, avoiding unnecessary small steps.

## RK1–RK2 Custom Method

An alternative pair that prioritizes efficient slope reuse:

- **RK1 (Euler)**: $\tilde{y}_{n+1} = y_n + h \, f(t_n, y_n)$.
- **RK2 (Midpoint)**: $\hat{y}_{n+1} = y_n + h \, f(t_n + h/2, y_n + \frac{h}{2} f(t_n, y_n))$.

### Structure and Efficiency

By computing in midpoint form, only two RHS evaluations are needed:

$$\displaystyle k_1 = f(t_n, y_n),$$

$$\displaystyle k_2 = f\left(t_n + \frac{h}{2}, y_n + \frac{h}{2} k_1\right),$$

$$\displaystyle \tilde{y}_{n+1} = y_n + h \, k_1 \quad \text{(Euler: order 1)},$$

$$\displaystyle \hat{y}_{n+1} = y_n + h \, k_2 \quad \text{(Midpoint RK2: order 2)}.$$

This halves the cost of the embedded pair compared to Heun–Euler (no extra function evaluation for high-order method). The error estimate is

$$\displaystyle e_n = \| \hat{y}_{n+1} - \tilde{y}_{n+1} \| = h \| k_2 - k_1 \|.$$

This method is ideal for problems where function evaluation is the dominant cost.

## Implementation and Usage

Both adaptive solvers are implemented in:

- **Heun–Euler**: [adaptive-common.hpp](../../../../includes/solvers/ODE/adaptive/adaptive-common.hpp)
- **RK1–RK2**: [adaptive-custom.hpp](../../../../includes/solvers/ODE/adaptive/adaptive-custom.hpp)

### Example: Adaptive Heun–Euler Solver

```C++
#include "path/to/includes/solvers/ODE/adaptive/adaptive-common.hpp"

// Define an ODE: y' = -y (exponential decay)
MyFunc f = [](double t, const dVec& y) -> dVec {
    dVec res(y.size());
    for (size_t i = 0; i < y.size(); ++i)
        res[i] = -y[i];
    return res;
};

// Set up adaptive solver parameters
SolverParameters params;
params.derivative = f;
params.initial_conditions = {1.0};
params.t0 = 0.0;
params.t1 = 5.0;
params.dt = 0.1;                    // Initial step size
params.error_estimate = true;       // Enable error estimation
params.variable_steps = true;       // Enable adaptive stepping
params.tolerance = 1e-6;            // Target tolerance per step
params.max_trial = 10;              // Max retries per step

// Solve
SolverResults results = heun_euler_solver(params);

// Access results
dMatrix solutions = results.solution;
dVec times = results.time_points;
dVec errors = results.errors;          // Error per step
dVec steps = results.steps;            // Step sizes used
```

### Example: Adaptive RK1–RK2 Custom Solver

```C++
#include "path/to/includes/solvers/ODE/adaptive/adaptive-custom.hpp"

// Same ODE: y' = -y
MyFunc f = [](double t, const dVec& y) -> dVec {
    dVec res(y.size());
    for (size_t i = 0; i < y.size(); ++i)
        res[i] = -y[i];
    return res;
};

SolverParameters params;
params.derivative = f;
params.initial_conditions = {1.0};
params.t0 = 0.0;
params.t1 = 5.0;
params.dt = 0.1;
params.error_estimate = true;
params.variable_steps = true;
params.tolerance = 1e-6;
params.max_trial = 10;

// Solve using RK1–RK2 pair (more efficient)
SolverResults results = rk12_custom_solver(params);
```

## Configuration and Parameters

The **SolverParameters** struct controls adaptive behavior:

```text
| Parameter | Type | Default | Description |
|-----------|------|---------|-------------|
| `derivative` | `MyFunc` | — | ODE RHS function $f(t, y)$ |
| `initial_conditions` | `dVec` | — | Initial value $y_0$ |
| `t0` | `double` | — | Start time |
| `t1` | `double` | — | End time |
| `dt` | `double` | — | Initial (and fixed) step size |
| `error_estimate` | `bool` | `false` | Compute per-step error estimates |
| `variable_steps` | `bool` | `false` | Adapt step size if `error_estimate=true` |
| `tolerance` | `double` | `1e-6` | Target error per step ($\varepsilon$) |
| `max_trial` | `size_t` | 10 | Max retry attempts per step |
| `attempts_history` | `bool` | `false` | Record all failed attempts (for diagnostics) |
```

**Results** structure populated by solvers:

```text
| Field | Type | Description |
|-------|------|-------------|
| `solution` | `dMatrix` | Solution $y$ at each time point (rows = time step, cols = components) |
| `time_points` | `dVec` | Time values $t_n$ |
| `errors` | `dVec` | Estimated error per step (if `error_estimate=true`) |
| `steps` | `dVec` | Step sizes used (if `variable_steps=true`) |
| `attempts_info` | (optional) | Failed attempt counts and histories (if `attempts_history=true`) |
```

## Practical Remarks

- **Tolerance selection**: Choose `tolerance` slightly less than desired accuracy in the final solution. A typical range is $[10^{-8}, 10^{-5}]$ for double precision.
- **Initial step size**: Start with a reasonable guess; the adaptive controller will quickly adjust if too large or too small.
- **Vector-valued problems**: All operations apply component-wise; use the component-wise (max or Euclidean) norm for error estimation.
- **Comparison with fixed-step RK**: Fixed-step RK methods are simpler and faster for smooth problems but can waste effort or fail on problems with varying regularity; adaptive methods excel when the solution regularity varies significantly over $[t_0, t_1]$.
- **Stability note**: Both Heun and RK2 are explicit; they inherit the stability restrictions of explicit Runge–Kutta methods. For stiff systems, consider implicit or implicit–explicit (IMEX) methods.

## Notes

- Fixed-step assumption: derivations assume constant (or adaptive) step sizes $h = t_{n+1}-t_n$ as specified.
- Embedded pair convention: the higher-order method provides the accepted solution; the lower-order is used only for error estimation.
- Implementation details: error control loops, step adjustment, and retry logic are in the source headers; see comments there for fine-tuning parameters.

## Other solvers

Beyond Heun–Euler and RK1–RK2, the library provides several higher-order embedded pairs for improved accuracy and efficiency on smooth problems. These are organized into two categories: standard methods in [adaptive-common.hpp](../../../../includes/solvers/ODE/adaptive/adaptive-common.hpp) and custom pairs in [adaptive-custom.hpp](../../../../includes/solvers/ODE/adaptive/adaptive-custom.hpp).

### Standard Embedded Pairs ([adaptive-common.hpp](../../../../includes/solvers/ODE/adaptive/adaptive-common.hpp))

#### Bogacki–Shampine (RK2(3))

The **RK2(3)** pair combines RK2 (order 2) with RK3 (order 3), using four function evaluations. This method is efficient for modest accuracy requirements and serves as a practical middle ground between Heun–Euler and higher-order methods.

**Solver function**: `bogacki_shampine_solver`

#### Fehlberg (RK3(4))

The **RK3(4)** pair combines RK3 (order 3) with RK4 (order 4), using five function evaluations. Fehlberg's method is a classical adaptive solver with good performance on non-stiff, moderately smooth problems.

**Solver function**: `fehlberg34_solver`

#### Dormand–Prince (RK4(5))

The **RK4(5)** pair is an optimization of Runge–Kutta–Fehlberg that re-uses slope evaluations more efficiently, providing superior accuracy-per-cost. It uses seven function evaluations and includes an embedded order-4 pair for error estimation. Widely used in scientific computing.

**Solver function**: `dormand_prince_solver`

### Custom Embedded Pairs ([adaptive-custom.hpp](../../../../includes/solvers/ODE/adaptive/adaptive-custom.hpp))

The library also provides custom embedded pairs optimized for efficient slope reuse:

#### RK2–RK3 Custom

Combines RK2 (Midpoint, order 2) with RK3, reusing intermediate slope evaluations. Ideal for problems where function evaluation dominates cost.

**Solver function**: `rk23_custom_solver`

#### RK3–RK4 Custom

Pairs RK3 (order 3) with RK4 (order 4), offering flexible trade-offs between efficiency and accuracy.

**Solver function**: `rk34_custom_solver`

#### RK4–RK5 Custom

Combines RK4 (order 4) with RK5 (order 5) using efficient slope reuse for high-accuracy adaptive integration.

**Solver function**: `rk45_custom_solver`

### Usage

All solvers follow the same calling convention as Heun–Euler and RK1–RK2, with identical `SolverParameters` and `SolverResults` structures. Select a solver based on desired accuracy and cost trade-off:

- **Low accuracy, efficiency priority**: Bogacki–Shampine or RK2–RK3 Custom.
- **Medium accuracy**: Fehlberg or RK3–RK4 Custom.
- **High accuracy**: Dormand–Prince or RK4–RK5 Custom.

All explicit methods share stability restrictions; they are suitable for non-stiff problems. For stiff systems, consider implicit or implicit–explicit (IMEX) methods from the stiff solvers documentation.

### Other methods (not implemented yet)

Here is a list of methods that are yet to be implemented (if you are willing to implement them please make sure to follow the same style in both documentation and implementation):

- **Runge–Kutta–Fehlberg (RK5(6))**: Higher-order variant using six stages for RK5 (order 5) and RK6 (order 6) pair; suitable for very high accuracy requirements.
- **Cash–Karp (RK4(5))**: Alternative RK4(5) pair with different stage coefficients; optimized for parallel evaluation on some architectures.
- **Tsitouras (RK5(4))**: Improved Dormand–Prince variant with better error constant; provides superior performance on some smooth problems.
- **Verner (RK6(5) and RK8(7))**: High-order explicit embedded pairs with optimized coefficients for very smooth or long-time integrations.
- **Merson (RK4(5))**: Classical adaptive method with five stages; historical importance and simplicity in coefficient structure.
- **Adams–Bashforth–Moulton Adaptive**: Multistep methods with variable step size and automatic order selection for stiff or smooth long-time integration.
- **BDF (Backward Differentiation Formulas) Adaptive**: Implicit multistep methods with adaptive order and step size; ideal for stiff problems.
- **Implicit Runge–Kutta (IRK) – Gauss–Legendre**: Fully implicit methods with optimal accuracy and stability for stiff systems.
- **Implicit Runge–Kutta (IRK) – Radau IIA**: High-order implicit methods with superior stability properties for stiff and moderately stiff problems.
- **Lobatto methods (IIIA, IIIB, IIIC)**: Implicit methods using Lobatto quadrature nodes with A- and L-stability properties.
- **SDIRK (Singly Diagonally Implicit Runge–Kutta)**: Semi-implicit methods with simplified linear system structure for moderately stiff problems.
- **Rosenbrock methods**: Linearly implicit methods avoiding full Newton iteration; efficient for mildly stiff problems with adaptive stepping.
- **W-methods**: Simplified Rosenbrock variants with reduced Jacobian computation overhead.
- **Extrapolation methods**: Richardson-based acceleration with embedded error control; efficient for smooth, non-stiff problems requiring very high accuracy.
- **Nystrom methods**: Specialized adaptive methods for second-order ODEs without conversion to first-order systems.
- **Exponential Runge–Kutta (ExpRK)**: Designed for semilinear problems with linear operator; more accurate on highly oscillatory or exponentially growing solutions.
- **Chebyshev explicit methods**: Stability-enhanced explicit methods optimized for problems with large negative real-part spectra (near-hyperbolic systems).
