# Adaptive Solvers' Utility

The adaptive solvers need functions to calculate the errors, adjust the next $\mathrm{d}\,t$ candidate and so on. These functions are implemented in the `adaptive-solvers-utility.hpp` file. They are not meant to be used outside of the adaptive solvers, but they are separated from the main implementation of the solvers for better organization and readability. If you need to add any utility function that is specific to the adaptive solvers, please add it to this file and document it properly. Also add the corresponding documentation to the `docs` directory, i.e. this file. (Please follow the same style and aesthetic while adding/contributing to the project.)

## Table of Contents

1. [Adaptive Solvers' Utility](#adaptive-solvers-utility)
    1. [Table of Contents](#table-of-contents)
    2. [Error Calculation](#error-calculation)
    3. [Step Size Adjustment](#step-size-adjustment)

## Error Calculation

The function of *error calculation* is required in all adaptive solvers so for ease of use, better readability, and organisation's sake I decided to implement it separately.

**Function Name:** `estimate_error`

**Purpose:** This function estimates the error between the current solution and a reference solution (which can be a higher-order method's solution or a more accurate solution). It supports different error estimation strategies, including weighted, component-wise error, and L2 norm-based error.

**Input:**

- `const dVec& y`: This is the current solution vector at the current time step.
- `const dVec& y_ref`: This is the current reference solution at the current time step.
- `const SolverParameters& Params`: This is the parameter `struct` that is provided for the solver input.

**Output:** It calculates and outputs a `double` value as the *error* for the current time step, which can be used to adjust the step size for the next iteration (till we reach a desirable value of error and move on to the next time step). This function will be used if `Params.error_estimate` is set as `true`.

**Implementation:**

```C++
// Helper function for error estimation
double estimate_error(const dVec& y, const dVec& y_ref, const SolverParameters& Params)
{
    double       error   = 0.0;
    const double epsilon = std::max(1e-15, Params.absolute_tol);
    double       denom   = epsilon;
    if (Params.weighted_error)
    {
        // Weighted component-wise error
        for (size_t i = 0; i < y.size(); ++i)
        {
            double diff = std::abs(y[i] - y_ref[i]);
            denom = Params.absolute_tol + std::max(std::abs(y_ref[i]), std::abs(y[i]));
            if (denom < epsilon) denom = epsilon; // avoid division by zero
            error = std::max(error, diff / denom);
        }
    }
    else if (Params.norm_error)
    {
        // L2 norm-based error
        double norm_diff = 0.0;
        double norm_sol  = 0.0;
        for (size_t i = 0; i < y.size(); ++i)
        {
            double diff = y[i] - y_ref[i];
            norm_diff += diff * diff;
            norm_sol += y_ref[i] * y_ref[i];
        }
        denom = Params.absolute_tol + std::sqrt(norm_sol);
        if (denom < epsilon) denom = epsilon; // avoid division by zero
        error = std::sqrt(norm_diff) / denom;
    }
    else
    {
        for (size_t i = 0; i < y.size(); ++i)
        {
            double diff = std::abs(y[i] - y_ref[i]);
            denom = Params.absolute_tol + std::abs(y_ref[i]);
            if (denom < epsilon) denom = epsilon; // avoid division by zero
            error = std::max(error, diff / denom);
        }
    }
    return error;
}
```

**Calculation:**

There are three strategies implemented here:

1. `weighted error`: The error is calculated with this formula if `weighted_error` is set as `true`. The formula is:

    $$ \displaystyle \text{error} = \max_{i} \left( \frac{|y_i - y_{\text{ref}, i}|}{\text{tol}_{\text{abs}} + \max(|y_{\text{ref}, i}|, |y_i|)} \right) $$

    This approach scales the error by the maximum of the reference and current solution components, providing good error control for both large and small solution values.

2. `norm error`: The error is calculated with this formula if `norm_error` is set as `true`. The formula is:

    $$ \displaystyle \text{error} = \frac{\sqrt{\sum_{i=0}^{n-1} (y_i - y_{\text{ref}, i})^2}}{\text{tol}_{\text{abs}} + \sqrt{\sum_{i=0}^{n-1} y_{\text{ref}, i}^2}} $$

    This is an L2 norm-based error that considers the overall magnitude of the error vector relative to the solution magnitude.

3. `component-wise error` (default): The error is calculated with this formula when neither `weighted_error` nor `norm_error` is set. The formula is:

    $$ \displaystyle \text{error} = \max_{i} \left( \frac{|y_i - y_{\text{ref}, i}|}{\text{tol}_{\text{abs}} + |y_{\text{ref}, i}|} \right) $$

    This strategy computes the maximum relative error across all components, scaling by the reference solution value. It provides component-by-component error control.

In all three strategies, $\text{tol}_{\text{abs}}$ is the absolute tolerance parameter (`Params.absolute_tol`), and the tolerance is always bounded below by a machine epsilon value ($1 \times 10^{-15}$) to avoid division by zero.

## Step Size Adjustment

The function of *step size adjustment* is required in all adaptive solvers so for ease of use, better readability, and organisation's sake I decided to implement it separately.

**Function Name:** `adjust_step_size`

**Purpose:** This function adjusts the step size for the next iteration based on the estimated error and the provided solver parameters. It uses a common adaptive step size control strategy that increases or decreases the step size based on whether the error is above or below the specified tolerance. The step size is bounded between `min_dt` and `max_dt` to ensure stability and efficiency. This function will be used if `Params.variable_steps` is set as `true`, it will be repeated until a desirable step size is found for the next iteration (that results in an acceptable error value) **OR** the number of trials for the current step is reached `Params.max_trials` (a safety feature to avoid infinite iterations).

**Input:**

- `double current_dt`: This is the current step size used in the iteration.
- `double error`: This is the estimated error for the current iteration, calculated using the `estimate_error` function.
- `const SolverParameters& Params`: This is the parameter `struct` that is provided for the solver.
- `size_t p`: This is the order of the solver method, which is used to optimally scale the error correction term when adjusting the step size.

**Output:** Returns a `double` value as the time step candidate for the next iteration of the current step.

**Implementation:**

```C++
// Helper function for step size adjustment
double adjust_step_size(double current_dt, double error, const SolverParameters& Params, size_t p)
{
    if (error > Params.local_tol && current_dt > Params.min_dt)
    {
        return std::max(std::min(current_dt * Params.decrease_factor * std::pow(Params.local_tol / error, 1.0 / (p + 1.0)), 
                                Params.max_dt), 
                       Params.min_dt);
    }
    else if (error < Params.local_tol * Params.local_tol_error_ratio || current_dt <= Params.min_dt)
    {
        return std::max(std::min(current_dt * Params.increase_factor * std::pow(Params.local_tol / error, 1.0 / (p + 1.0)), 
                                Params.max_dt), 
                       Params.min_dt);
    }
    return current_dt;
}
```

**Calculations:**

Time step value for the next iteration of the same step is calculated within this function, using the following formula:

$$\displaystyle \text{new\_dt} = \begin{cases}
\max\left(\min\left(\text{current\_dt} \cdot \text{decrease\_factor} \cdot \left(\frac{\text{tol}_{\text{local}}}{\text{error}}\right)^{\frac{1}{p+1}}, \text{max\_dt}\right), \text{min\_dt}\right) & \text{if } \text{error} > \text{tol}_{\text{local}} \text{ and } \text{current\_dt} > \text{min\_dt} \\[0.5em]
\max\left(\min\left(\text{current\_dt} \cdot \text{increase\_factor} \cdot \left(\frac{\text{tol}_{\text{local}}}{\text{error}}\right)^{\frac{1}{p+1}}, \text{max\_dt}\right), \text{min\_dt}\right) & \text{if } \text{error} < \text{tol}_{\text{local}} \cdot \text{tol\_error\_ratio} \text{ or } \text{current\_dt} \leq \text{min\_dt} \\[0.5em]
\text{current\_dt} & \text{otherwise}
\end{cases}$$

**Description of the step size adjustment strategy:**

The step size adjustment mechanism has three cases:

1. **Error too large (decrease step size)**: When the estimated error exceeds the local tolerance threshold (`error > tol_local`) and the current step size is still above the minimum allowed value (`current_dt > min_dt`), the step size is decreased using the formula:

   $$\displaystyle \text{new\_dt} = \max\left(\min\left(\text{current\_dt} \cdot \text{decrease\_factor} \cdot \left(\frac{\text{tol}_{\text{local}}}{\text{error}}\right)^{\frac{1}{p+1}}, \text{max\_dt}\right), \text{min\_dt}\right)$$

   where $p$ is the order of the solver method. The factor $\left(\frac{\text{tol}_{\text{local}}}{\text{error}}\right)^{\frac{1}{p+1}}$ provides a proportional correction based on how much the error exceeds the tolerance, scaled by the solver's order for optimal convergence.

2. **Error too small (increase step size)**: When the estimated error is well below the tolerance threshold (`error < tol_local * tol_error_ratio`) or when the step size has already reached its minimum limit, the step size is increased using:

   $$\displaystyle \text{new\_dt} = \max\left(\min\left(\text{current\_dt} \cdot \text{increase\_factor} \cdot \left(\frac{\text{tol}_{\text{local}}}{\text{error}}\right)^{\frac{1}{p+1}}, \text{max\_dt}\right), \text{min\_dt}\right)$$

3. **Error acceptable**: When the error is within the acceptable range, the step size remains unchanged.

**Parameter definitions:**

- $\text{tol}_{\text{local}}$ = `Params.local_tol`: The local tolerance threshold for acceptable error.
- $\text{tol\_error\_ratio}$ = `Params.local_tol_error_ratio`: A multiplier defining the lower bound for acceptable error (e.g., 0.01 means errors below 1% of the tolerance trigger step size increase).
- $\text{decrease\_factor}$ = `Params.decrease_factor`: Multiplication factor when decreasing step size (typically < 1, e.g., 0.8).
- $\text{increase\_factor}$ = `Params.increase_factor`: Multiplication factor when increasing step size (typically > 1, e.g., 1.2).
- $\text{min\_dt}$ and $\text{max\_dt}$ = `Params.min_dt` and `Params.max_dt`: Bounds on the step size to ensure numerical stability and efficiency.
- $p$ = Order of the solver method, used to optimally scale the error correction term.
