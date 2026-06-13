# Runge-Kutta 2 (Midpoint Method)

The Runge-Kutta 2 method, commonly known as the midpoint method or the improved Euler method, is a second-order explicit method for solving ordinary differential equations (ODEs). It belongs to the Runge-Kutta family and provides better accuracy than Euler's method (RK1) by employing two stages of function evaluations. It solves initial value problems of the form:

$$ \displaystyle \frac{dy}{dt} = f(t, y), \quad y(t_0) = y_0 $$

where $ f(t, y) $ is a given function, $ t_0 $ is the initial time, and $ y_0 $ is the initial value of the function $ y $.

## Table of Contents

1. [Runge-Kutta 2 (Midpoint Method)](#runge-kutta-2-midpoint-method)
    1. [Table of Contents](#table-of-contents)
    2. [Derivation and Algorithm](#derivation-and-algorithm)
        1. [Accuracy and Error](#accuracy-and-error)
    3. [Example Usage](#example-usage)

## Derivation and Algorithm

The Runge-Kutta 2 method improves upon Euler's method by evaluating the right-hand side function at an intermediate point. Instead of using the slope at the beginning of the interval, RK2 estimates the slope at the midpoint, which provides a better approximation.

**Motivation via Taylor expansion:**

Starting from the Taylor series expansion around $t_n$:

$$ \displaystyle y(t_{n+1}) = y(t_n) + y'(t_n) \mathrm{d}t + \frac{1}{2}y''(t_n) \mathrm{d} t^2 + \frac{1}{6}y'''(\xi) \mathrm{d} t^3, $$

we know that $ y'(t_n) = f(t_n, y_n) $ and $ y''(t_n) = \frac{\partial f}{\partial t} + \frac{\partial f}{\partial y} f $ at $(t_n, y_n)$.

The RK2 method approximates this expansion to second order by using two slope evaluations:

1. **First stage (k1):** Evaluate the slope at the current point
   $$ \displaystyle k_1 = f(t_n, y_n) $$

2. **Second stage (k2):** Evaluate the slope at the midpoint
   $$ \displaystyle k_2 = f\left(t_n + \frac{dt}{2}, y_n + \frac{dt}{2} k_1\right) $$

3. **Update step:** Use the midpoint slope to advance the solution
   $$ \displaystyle y_{n+1} = y_n + dt \, k_2. $$

This is equivalent to:

$$ \displaystyle y_{n+1} = y_n + \mathrm{d}t f\left(t_n + \frac{dt}{2}, y_n + \frac{dt}{2} f(t_n, y_n)\right). $$

**Why the midpoint improves accuracy:** - The midpoint evaluation captures the curvature of the solution better than a single evaluation at the start or end of the interval. The use of $k_2$ at the midpoint $\left(t_n + \frac{dt}{2}\right)$ provides a weighted average that approximates the solution behavior over the entire interval.

**Algorithm:**

1. Initialize the variables: Set the initial conditions $t_0$, $y_0$, and choose a step size $dt$.
2. For each step from $n = 0$ to the desired number of steps:
    1. Calculate the first slope: $k_1 = f(t_n, y_n)$
    2. Calculate the midpoint state: $\tilde{y} = y_n + \frac{dt}{2} k_1$
    3. Calculate the second slope at the midpoint: $k_2 = f\left(t_n + \frac{dt}{2}, \tilde{y}\right)$
    4. Update the solution using the midpoint slope: $y_{n+1} = y_n + dt \, k_2$
    5. Advance time: $t_{n+1} = t_n + dt$
3. Output the calculated values at each time point.

This is a second-order explicit Runge-Kutta ODE solver. It is implemented at [includes/solvers/ODE/rk/explicit/rk2-solver.hpp](../../../includes/solvers/ODE/rk/explicit/rk2-solver.hpp)

### Accuracy and Error

RK2 is a *second-order* method: the local truncation error per step is $\mathcal{O}(dt^3)$ while the global error accumulated over many steps is $\mathcal{O}(dt^2)$. This quadratic convergence makes RK2 significantly more accurate than RK1 (Euler's method) and suitable for problems where moderate accuracy is required without the overhead of fourth-order methods like RK4.

Let $\tilde{y}(t)$ denote the exact solution and $y_n$ the RK2 approximation at $t_n$. The local truncation error (LTE) at a single step can be derived by comparing the RK2 update with the Taylor series expansion:

$$ \displaystyle LTE_n = \tilde{y}(t_{n+1}) - y_{n+1} = \frac{1}{6}y'''(\xi) \mathrm{d} t^3 = \mathcal{O}(dt^3), $$

for some $\xi \in [t_n, t_{n+1}]$. Summing these local errors over $N \approx (t_1 - t_0)/dt$ steps gives the global truncation error (GTE)

$$ \displaystyle GTE_N = \tilde{y}(t_N) - y_N = \sum_{n=0}^{N-1} LTE_n = \mathcal{O}(dt^2). $$

**Rigorous derivation of the LTE:**

To understand why the LTE is $\mathcal{O}(dt^3)$, we expand $k_2$ using a two-variable Taylor series. Let $y_n^* = y_n + \frac{dt}{2} f(t_n, y_n)$ denote the midpoint state. Then

$$ \displaystyle k_2 = f\left(t_n + \frac{dt}{2}, y_n^*\right) = f(t_n, y_n) + \frac{\partial f}{\partial t}\bigg|_{(t_n,y_n)} \frac{dt}{2} + \frac{\partial f}{\partial y}\bigg|_{(t_n,y_n)} \frac{dt}{2} f(t_n, y_n) + \mathcal{O}(dt^2). $$

Substituting $f_t + f_y f = y''$, we get

$$ \displaystyle k_2 = f(t_n, y_n) + \frac{dt}{2} y''(t_n) + \mathcal{O}(dt^2). $$

Therefore, the RK2 update is

$$ \displaystyle y_{n+1}^{RK2} = y_n + dt \, k_2 = y_n + dt \, f(t_n, y_n) + \frac{dt^2}{2} y''(t_n) + \mathcal{O}(dt^3). $$

Comparing with the exact Taylor series

$$ \displaystyle \tilde{y}(t_{n+1}) = y_n + dt \, f(t_n, y_n) + \frac{dt^2}{2} y''(t_n) + \frac{dt^3}{6} y'''(t_n) + \mathcal{O}(dt^4), $$

we have

$$ \displaystyle LTE_n = \tilde{y}(t_{n+1}) - y_{n+1}^{RK2} = \frac{dt^3}{6} y'''(t_n) + \mathcal{O}(dt^4) = \mathcal{O}(dt^3). $$

**Third derivative (useful for LTE estimates)** - For a scalar ODE $y' = f(t, y)$, the third derivative of the solution is computed by differentiating $y''(t)$ along the solution:

$$ \displaystyle y'''(t) = \frac{\mathrm{d}}{\mathrm{d}t}\left( f_t + f_y f \right) = f_{tt} + 2 f_{ty} f + f_{yy} f^2 + f_y f_t + f_y^2 f, $$

which simplifies to

$$ \displaystyle y'''(t) = f_{tt} + 2 f_{ty} f + f_{yy} f^2 + f_y (f_t + f_y f) = f_{tt} + 2 f_{ty} f + f_{yy} f^2 + f_y \, y''. $$

For a vector-valued state $y \in \mathbb{R}^m$ with $f: \mathbb{R} \times \mathbb{R}^m \to \mathbb{R}^m$, the same structure holds where second derivatives are Hessian matrices and products are matrix-vector operations.

**Stability and stiffness (practical note)** - Consider the linear test equation $y' = \lambda y$ with (possibly complex) eigenvalue $\lambda$. Applying RK2 yields

$$\tilde{y} = y_n + \frac{dt}{2} \lambda y_n = \left(1 + \frac{\lambda \mathrm{d} t}{2}\right) y_n,$$

$$y_{n+1} = y_n + dt \, \lambda \, \tilde{y} = y_n + dt \, \lambda \left(1 + \frac{\lambda \mathrm{d} t}{2}\right) y_n = \left(1 + \lambda \mathrm{d} t + \frac{(\lambda \mathrm{d} t)^2}{2}\right) y_n.$$

Stability of the discrete scheme requires $|1 + \lambda \mathrm{d} t + \frac{(\lambda \mathrm{d} t)^2}{2}| < 1$. The stability region is larger than RK1 but still bounded (not A-stable). Stiffness considerations are similar to RK1:

- RK2 is explicit and conditional stability; for stiff problems with large negative real parts of eigenvalues, very small $dt$ may be required.
- Implicit methods are preferred for truly stiff systems.

For moderately stiff or non-stiff problems, RK2 provides a good balance between accuracy and computational cost.

## Example Usage

For the following system

$$ \displaystyle \dot{y}(t) = \alpha y(t). $$

where $ y(0) = 1.0, \: \alpha = 0.9, \: t_0 = 0.0, \: t_1 = 20.0, \: dt = 1.0/32.0 $, the following snippet shows how the library can be used to solve it.

```C++
#include "path/to/includes/solvers/ODE/rk/explicit/rk2-solver.hpp"

// Wrapper function that creates a MyFunc with alpha baked in
MyFunc createODEFunction(double alpha)
{
    return [alpha](double t, const dVec& y) -> dVec
    {
        dVec res(y.size());
        for (size_t i = 0; i < y.size(); ++i)
        {
            res[i] = alpha * y[i];
        }
        return res;
    };
}

// Set initial conditions
dVec y0 = {1.0};  // y(0) = 1.0

// Create the ODE function with alpha = 0.9
MyFunc f = createODEFunction(0.9);

// Method 1: Using SolverParameters struct
SolverParameters params;
params.derivative = f;
params.initial_conditions = y0;
params.t0 = 0.0;
params.t1 = 20.0;
params.dt = 1.0 / 32.0;

SolverResults results = rk2_solver(params);
dMatrix solutions = results.solution;
dVec times = results.time_points;

// Method 2: Using convenience overload with inline function creation
dMatrix solutions_alt = rk2_solver(createODEFunction(0.9), y0, 0.0, 20.0, 1.0 / 32.0);

// Analytical solution for comparison: y(t) = y0 * exp(alpha * t)
// y(20) = 1.0 * exp(0.9 * 20) ≈ 6.96e+07
```
