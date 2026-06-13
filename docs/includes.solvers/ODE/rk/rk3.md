<!-- markdownlint-disable MD018 MD051 -->
# Runge-Kutta 3 (Kutta's Third-Order Method)

The Runge-Kutta 3 method, also known as Kutta's third-order method, is a third-order explicit method for solving ordinary differential equations (ODEs). It represents a significant improvement in accuracy over RK2 while remaining computationally efficient. The method uses three stages of function evaluations to approximate the solution. It solves initial value problems of the form:

$$ \displaystyle \frac{\mathrm{d}y}{\mathrm{d}t} = f(t, y), \quad y(t_0) = y_0 $$

where $ f(t, y) $ is a given function, $ t_0 $ is the initial time, and $ y_0 $ is the initial value of the function $ y $.

## Table of Contents

1. [Runge-Kutta 3 (Kutta's Third-Order Method)](#runge-kutta-3-kuttas-third-order-method)
    1. [Table of Contents](#table-of-contents)
    2. [Derivation and Algorithm](#derivation-and-algorithm)
        1. [Accuracy and Error](#accuracy-and-error)
    3. [Example Usage](#example-usage)

## Derivation and Algorithm

Imagine $ f(t, y) $ is a function that defines the rate of change of $ y $ with respect to $ t $. The Runge-Kutta 3 method approximates the solution at each step using three evaluations of this function.

First, we define the stages:

1. **First stage (k1):** Evaluate the slope at the current point
   $$ \displaystyle k_1 = f(t_n, y_n) $$

2. **Second stage (k2):** Evaluate the slope at the midpoint using k1
   $$ \displaystyle k_2 = f\left(t_n + \frac{\mathrm{d} t}{2}, y_n + \frac{\mathrm{d} t}{2} k_1\right) $$

3. **Third stage (k3):** Evaluate the slope at the end of the interval using k2
   $$ \displaystyle k_3 = f\left(t_n + \mathrm{d} t, y_n + \mathrm{d} t \, k_2\right) $$

4. **Update step:** Combine the slopes to update the solution
   $$ \displaystyle y_{n+1} = y_n + \frac{\mathrm{d} t}{6} (k_1 + 4k_2 + k_3). $$

The derivation of the $m^{th}$ order **RK** method is as follows.

**Equation system and notation:**

$$ \displaystyle \frac{\mathrm{d}^n y}{\mathrm{d} t^n} \equiv y^{(n)} $$

**ODE (first order):**

$$ \displaystyle y' = f(t, y) $$

**Taylor expansion around $t_n$:**

$$ \displaystyle y(t_{n+1}) = y(t_n) + y'(t_n) \mathrm{d} t + \frac{1}{2}y''(t_n) \mathrm{d} t^2 + \frac{1}{6}y'''(t_n) \mathrm{d} t^3 + \ldots + \frac{1}{m!}y^{(m)}(t_n) \mathrm{d} t^m + \frac{1}{(m+1)!}y^{(m+1)}(\xi) \mathrm{d} t^{m+1}, $$

**The Runge-Kutta formulation:**

Imagine the following general formulation for an $m^{th}$ order RK method,

$$ \displaystyle y_{n+1} = y_n + \mathrm{d} t \sum_{i=1}^{m} b_i k_i, $$

where,

$$ \displaystyle k_i = f\left(t_n + c_i \mathrm{d} t, y_n + \mathrm{d} t \sum_{j=1}^{i-1} a_{ij} k_j\right). $$

To derive the coefficients for the RK3 method, we match the Taylor series expansion of the exact solution with the RK formulation up to third order. This involves expanding both sides and equating coefficients of like powers of $\mathrm{d} t$.

The necessary steps are:

$$ \displaystyle y' = f(t,y), $$

$$ \displaystyle y'' = \partial_{t} f(t,y) + \partial_{y} f(t,y) y', $$

$$ \displaystyle y''' = \partial_{tt} f(t,y) + 2 \partial_{ty} f(t,y) y' + \partial_{yy} f(t,y) (y')^2 + \partial_{y} f(t,y) y''. $$

$$ \displaystyle y(t) = y(t_0) + f(t_0, y_0) (t - t_0) + \frac{1}{2} \left[ \partial_{t} f(t_0,y_0) + \partial_{y} f(t_0,y_0) f(t_0,y_0) \right] (t - t_0)^2 + \frac{1}{6} \left[ \partial_{tt} f(t_0,y_0) + 2 \partial_{ty} f(t_0,y_0) f(t_0,y_0) + \partial_{yy} f(t_0,y_0) (f(t_0,y_0))^2 + \partial_{y} f(t_0,y_0) \left( \partial_{t} f(t_0,y_0) + \partial_{y} f(t_0,y_0) f(t_0,y_0) \right) \right] (t - t_0)^3 + \ldots . $$

Rearranging the similar orders on $\mathrm{d}t$ yields,

$$ \displaystyle y(t_{n+1}) = y(t_n) + f(t_n, y_n) \mathrm{d} t + \frac{1}{2} \left[ \partial_{t} f(t_n,y_n) + \partial_{y} f(t_n,y_n) f(t_n,y_n) \right] \mathrm{d} t^2 + \frac{1}{6} \left[ \partial_{tt} f(t_n,y_n) + 2 \partial_{ty} f(t_n,y_n) f(t_n,y_n) + \partial_{yy} f(t_n,y_n) (f(t_n,y_n))^2 + \partial_{y} f(t_n,y_n) \left( \partial_{t} f(t_n,y_n) + \partial_{y} f(t_n,y_n) f(t_n,y_n) \right) \right] \mathrm{d} t^3 + \ldots . $$

and the RK3 formulation expanded is,

$$ \displaystyle y_{n+1} = y_n + \mathrm{d} t \left( b_1 f(t_n, y_n) + b_2 f\left(t_n + c_2 \mathrm{d} t, y_n + \mathrm{d} t a_{21} f(t_n, y_n)\right) + b_3 f\left(t_n + c_3 \mathrm{d} t, y_n + \mathrm{d} t \left( a_{31} f(t_n, y_n) + a_{32} f\left(t_n + c_2 \mathrm{d} t, y_n + \mathrm{d} t a_{21} f(t_n, y_n)\right) \right) \right) \right). $$

By expanding the $f(t_n + c_i \mathrm{d}t, y_n + \mathrm{d}t \sum_{j=1}^{i-1} a_{ij} k_j)$ terms using Taylor series and matching coefficients of $\mathrm{d} t$, we derive the following coefficients for the RK3 method:

1. $ b_1 = \frac{1}{6} $
2. $ b_2 = \frac{2}{3} $
3. $ b_3 = \frac{1}{6} $
4. $ c_1 = 0 $
5. $ c_2 = \frac{1}{2} $
6. $ c_3 = 1 $
7. $ a_{21} = \frac{1}{2} $
8. $ a_{31} = -1 $
9. $ a_{32} = 2 $
10. All other $ a_{ij} = 0 $

**NOTE:** The derivation involves careful algebraic manipulation and is often facilitated by symbolic computation tools due to its complexity.

**NOTE (Butcher Tableau):** The coefficients can be summarized in the following Butcher tableau:

$$ \displaystyle \begin{array}{c|ccc}
0 & 0 & 0 & 0 \\
\frac{1}{2} & \frac{1}{2} & 0 & 0 \\
1 & -1 & 2 & 0 \\
\hline
 & \frac{1}{6} & \frac{2}{3} & \frac{1}{6} \end{array} $$

Using these coefficients, we can implement the RK3 method as it the following:

1. Initialize the variables: Set the initial conditions $t_0$, $y_0$, and choose a step size $\mathrm{d} t$.
2. For each step from $n = 0$ to the desired number of steps:
    1. Calculate the first slope: $k_1 = f(t_n, y_n)$
    2. Calculate the second slope: $k_2 = f\left(t_n + \frac{\mathrm{d} t}{2}, y_n + \frac{\mathrm{d} t}{2} k_1\right)$
    3. Calculate the third slope: $k_3 = f\left(t_n + \mathrm{d} t, y_n + \mathrm{d} t \, k_2\right)$
    4. Update the solution: $y_{n+1} = y_n + \frac{\mathrm{d} t}{6} (k_1 + 4k_2 + k_3)$
    5. Advance time: $t_{n+1} = t_n + \mathrm{d} t$
3. Output the calculated values at each time point.

### Accuracy and Error

The RK3 method is a *third-order* method: the local truncation error per step is $\mathcal{O}(\mathrm{d} t^4)$ while the global error accumulated over many steps is $\mathcal{O}(\mathrm{d} t^3)$. This cubic convergence makes RK3 significantly more accurate than both RK1 and RK2, making it suitable for problems where higher accuracy is required without the computational cost of fourth-order methods like RK4.

The enthusiast can derive the local truncation error (LTE) and global truncation error (GTE) similarly to the RK2 case, confirming the third-order accuracy of the RK3 method.

## Example Usage

This is a third-order explicit Runge-Kutta ODE solver. It is implemented at [includes/solvers/ODE/rk/explicit/rk3-solver.hpp](../../../includes/solvers/ODE/rk/explicit/rk3-solver.hpp)

```C++
//The following line includes the RK3 solver implementation
#include "path/to/includes/solvers/ODE/rk/explicit/rk3-solver.hpp"

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
params.\mathrm{d} t = 1.0 / 32.0;

SolverResults results = rk3_solver(params);
dMatrix solutions = results.solution;
dVec times = results.time_points;

// Method 2: Using convenience overload with inline function creation
dMatrix solutions_alt = rk3_solver(createODEFunction(0.9), y0, 0.0, 20.0, 1.0 / 32.0);

// Analytical solution for comparison: y(t) = y0 * exp(alpha * t)
// y(20) = 1.0 * exp(0.9 * 20) ≈ 6.96e+07

```
