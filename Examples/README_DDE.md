# Delay Differential Equation (DDE) Examples

This directory contains examples demonstrating the use of the `rk1_dde_solver` function for solving delay differential equations.

## Files

### 1. `dde_example.cpp`
A simple example demonstrating a basic linear delay differential equation:
- **Equation**: `dy/dt = -y(t) + 0.5 * y(t - 1.0)`
- **Initial condition**: `y(0) = 1.0`
- **History function**: `y(t) = 1.0` for `t < 0`
- **Delay time**: `τ = 1.0`
- **Time interval**: `[0, 10]`
- **Step size**: `dt = 0.1`

### 2. `dde_complex_example.cpp`
A more complex example with a nonlinear delay differential equation:
- **Equation**: `dy/dt = -2*y(t) + sin(y(t - 0.5)) + 0.3*y(t - 0.5)`
- **Initial condition**: `y(0) = 0.5`
- **History function**: `y(t) = sin(t)` for `t < 0`
- **Delay time**: `τ = 0.5`
- **Time interval**: `[0, 15]`
- **Step size**: `dt = 0.05`

## Compilation

To compile the examples, use:

```bash
# Simple example
g++ -std=c++20 -I../includes dde_example.cpp -o dde_example

# Complex example
g++ -std=c++20 -I../includes dde_complex_example.cpp -o dde_complex_example
```

## Running

```bash
# Run simple example
./dde_example

# Run complex example
./dde_complex_example
```

## Key Features Demonstrated

1. **DDE Function Definition**: Using `DelayFunc` type for functions of the form `f(t, y, y_delayed)`
2. **History Function**: Defining pre-history values for `t < t0`
3. **Delay Times**: Specifying delay parameters in the solver
4. **Linear Interpolation**: The solver automatically interpolates delayed values
5. **Parameter Configuration**: Setting up `SolverParameters` for DDE problems

## DDE Solver Interface

The `rk1_dde_solver` function expects:

```cpp
SolverResults rk1_dde_solver(const SolverParameters& params);
```

Where `SolverParameters` includes DDE-specific fields:
- `delay_derivative`: The DDE function `f(t, y, y_delayed)`
- `delay_times`: Vector of delay times `τ_i`
- `history_function`: Pre-history function for `t < t0`
- `is_dde`: Flag indicating this is a DDE problem

## Mathematical Background

Delay differential equations have the general form:
```
dy/dt = f(t, y(t), y(t - τ₁), y(t - τ₂), ..., y(t - τₙ))
```

where `τ_i` are the delay times. The solver uses linear interpolation to estimate `y(t - τ)` from previously computed solution values. 