# Runge-Kutta 1 (Euler's Method)

The Runge-Kutta 1 method, commonly known as Euler's method, is the simplest form of the explicit form of the Runge-Kutta family of methods for solving ordinary differential equations (ODEs). It is a first-order numerical procedure for solving initial value problems of the form:

$$ \displaystyle \frac{dy}{dt} = f(t, y), \quad y(t_0) = y_0 $$

where $ f(t, y) $ is a given function, $ t_0 $ is the initial time, and $ y_0 $ is the initial value of the function $ y $.

## Table of Contents

1. [Runge-Kutta 1 (Euler's Method)](#runge-kutta-1-eulers-method)
    1. [Table of Contents](#table-of-contents)
    2. [Derivation and Algorithm](#derivation-and-algorithm)
        1. [Accuracy and Error](#accuracy-and-error)
    3. [Example Usage](#example-usage)

## Derivation and Algorithm

Imagine that the RHS of the differential equation is given by a function `f(t,y)` and the function for most conventional systems is an analytical one. We also know the value of the *state variable*, i.e. `y(t)`, at a certain point of time $t_0$. For convenience's sake we can consider $t_0 = 0,\: y(t_0) = y_0$. We can expand the function `y(t)` in terms of the Taylor series around the point $t_0$ (assuming it is analytical as well):

$$ \displaystyle y(t) = y(t_0) + \frac{\mathrm{d} y}{\mathrm{d} t}\|_{t=t_0} (t - t_0) + \frac{1}{2} \frac{\mathrm{d}^2 y}{\mathrm{d} t^2}\|_{t=t_0} (t - t_0)^2 + \ldots .$$

We can truncate the series after a certain degree depending on the desired accuracy. If we truncate it after the first degree, we get:

$$  \displaystyle y(t) = y(t_0) + \frac{\mathrm{d} y}{\mathrm{d} t}\|_{t=t_0} (t-t_0).$$

and hence,

$$ \displaystyle y(t) = y_0 + f(t_0, y_0) (t - t_0). $$

Now assume $t = t_0 + dt$ which will justify the truncation for sufficiently small $dt$. We get:

$$ \displaystyle y(t_0 + dt) = y_0 + f(t_0, y_0) dt. $$

And doing the same for the next step we get:

$$ \displaystyle y(t_0 + 2dt) = y(t_0 + dt) + f\left(t_0+dt, y\left(t_0 + dt\right)\right) dt. $$

Or if we come up with the convention of, $t_n$ being the value of time (independent variable) after $n$ steps (for constant step size it would be $t_0+ndt$). The value of state variable y at the next step, $y(t_{n+1})\equiv y_{n+1}$, is given as:

$$ \displaystyle y_{n+1} = y_n + f(t_n, y_n) dt. $$

This final equation is generally called the *Euler's* method.

Now that we derived the fomula, we can simply apply it as an algorithm to solve ODEs. The algorithm is as follows:

1. Initialize the variables: Set the initial conditions $t_0$, $y_0$, and choose a step size $dt$.
2. For each step from $n = 0$ to the desired number of steps:
    1. Calculate the value of the next step based on the value of the current step.
    2. Set the value of the current state equal to the value calculated at first step.
    3. Repeat the two previous steps until you reach the desired $n$ or $t$.
3. Output the calculated values at each time point.

This is the first order explicit Runge-Kutta ODE solver. It is implemented at [includes/solvers/ODE/rk/explicit/rk1-solver.hpp](../../../includes/solvers/ODE/rk/explicit/rk1-solver.hpp)

### Accuracy and Error

Euler's method is a *first-order* method: the local truncation error per step is $\mathcal{O}(dt^2)$ while the global error accumulated over many steps is $\mathcal{O}(dt)$. This makes Euler suitable for quick, low-accuracy approximations but not for precision-sensitive problems; use higher-order Runge–Kutta methods (RK2, RK3, RK4) when accuracy matters.

Let $\tilde{y}(t)$ be the exact solution and $y_n$ the Euler approximation at $t_n$. The local truncation error (LTE) at a single step satisfies

$$ \displaystyle LTE_n = \tilde{y}(t_{n+1}) - y_{n+1} = \frac{1}{2}y''(\xi)\,dt^2 = \mathcal{O}(dt^2), $$

for some $\xi\in[t_n,t_{n+1}]$. Summing these local errors over $N\approx (t_1-t_0)/dt$ steps gives the global truncation error (GTE)

$$ \displaystyle GTE_N = \tilde{y}(t_N) - y_N = \sum_{n=0}^{N-1} LTE_n = \mathcal{O}(dt). $$

**Second derivative (useful for LTE estimates)** -  For a scalar ODE $y' = f(t,y)$ the second derivative of the solution can be written by differentiating the RHS along the solution:

$$ \displaystyle y''(t) = \frac{\mathrm{d}}{\mathrm{d}t}f(t,y(t)) = \frac{\partial f}{\partial t}(t,y(t)) + \frac{\partial f}{\partial y}(t,y(t))\,f(t,y(t)). $$

For a vector-valued state $y\in\mathbb{R}^m$ and $f:\mathbb{R}\times\mathbb{R}^m\to\mathbb{R}^m$, the same identity holds with the Jacobian $J_f=\partial f/\partial y$:

$$ \displaystyle y''(t) = f_t(t,y) + J_f(t,y)\,f(t,y), $$

where $f_t$ is the partial derivative of $f$ with respect to $t$ and $J_f$ is the $m\times m$ Jacobian matrix.

**Stability and stiffness (practical note)** - Consider the linear test equation $y' = \lambda y$ with (possibly complex) eigenvalue $\lambda$. Applying explicit Euler yields

$$ y_{n+1} = (1 + \lambda\mathrm{d}t)\,y_n. $$

Stability of the discrete scheme requires $|1+\lambda\mathrm{d}t|<1$. The set of $z=\lambda\mathrm{d}t$ satisfying this is the open disk in the complex plane centered at $-1$ with radius $1$ (the explicit Euler stability region). Consequently:

- If the continuous problem has eigenvalues with large negative real parts (stiff problems), explicit Euler requires very small $dt$ for stability.
- Euler is not A-stable; implicit methods (e.g., implicit Euler, backward differentiation formulas) are preferred for stiff problems because they allow larger stable step sizes.

When using Euler in practice, monitor stability (or choose an adaptive and/or implicit solver) for stiff systems or when eigenvalues of the Jacobian have large negative real parts.

## Example Usage

For the following system

$$ \displaystyle \dot{y}(t) = \alpha y(t). $$

where $ y(0) = 1.0, \: \alpha = 0.9, \: t_0 = 0.0, \: t_1 = 20.0, \: dt = 1.0/32.0 $, the following snippet shows how the library can be used to solve it.

```C++
#include "path/to/includes/solvers/ODE/rk/explicit/rk1-solver.hpp"

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

SolverResults results = rk1_solver(params);
dMatrix solutions = results.solution;
dVec times = results.time_points;

// Method 2: Using convenience overload with inline function creation
dMatrix solutions_alt = rk1_solver(createODEFunction(0.9), y0, 0.0, 20.0, 1.0 / 32.0);
```
