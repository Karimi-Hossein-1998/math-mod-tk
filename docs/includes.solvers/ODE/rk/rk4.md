<!-- markdownlint-disable MD018 MD051 -->
# Runge-Kutta 4 (Classical Fourth-Order Method)

The classical Runge–Kutta method of order four (RK4) is a widely used explicit method for numerically integrating ordinary differential equations (ODEs). It provides a good balance between accuracy and computational cost by using four derivative evaluations per step. RK4 solves initial value problems of the form

$$ \displaystyle \frac{\mathrm{d} y}{\mathrm{d} t} = f(t, y), \quad y(t_0) = y_0 $$

where $f(t,y)$ is given, $t_0$ is the initial time, and $y_0$ the initial state.

## Table of Contents

1. [Runge-Kutta 4 (Classical Fourth-Order Method)](#runge-kutta-4-classical-fourth-order-method)
    1. [Table of Contents](#table-of-contents)
    2. [Derivation and Algorithm](#derivation-and-algorithm)
        1. [Accuracy and Error](#accuracy-and-error)
        2. [Stability and stiffness (practical note)](#stability-and-stiffness-practical-note)
    3. [Example Usage](#example-usage)

## Derivation and Algorithm

RK4 advances the solution from $t_n$ to $t_{n+1}=t_n + \mathrm{d} t$ using four stage evaluations:

1. $\displaystyle k_1 = f(t_n, y_n)$
2. $\displaystyle k_2 = f\left(t_n + \frac{\mathrm{d} t}{2}, y_n + \frac{\mathrm{d} t}{2} k_1\right)$
3. $\displaystyle k_3 = f\left(t_n + \frac{\mathrm{d} t}{2}, y_n + \frac{\mathrm{d} t}{2} k_2\right)$
4. $\displaystyle k_4 = f\left(t_n + \mathrm{d} t, y_n + \mathrm{d} t\, k_3\right)$

The update formula is the weighted average

$$ \displaystyle y_{n+1} = y_n + \frac{\mathrm{d} t}{6}\left(k_1 + 2k_2 + 2k_3 + k_4\right). $$

This choice of stages and weights is obtained by matching the Taylor series expansion of the exact solution with the RK ansatz through order $\mathrm{d} t^4$ (the method is constructed so that terms up to $\mathrm{d} t^4$ coincide).

**Butcher tableau (classical RK4):**

$$\begin{array}{c|cccc}
0 & & & & \\
\frac{1}{2} & \frac{1}{2} & & & \\
\frac{1}{2} & 0 & \frac{1}{2} & & \\
1 & 0 & 0 & 1 & \\
\hline
& \frac{1}{6} & \frac{1}{3} & \frac{1}{3} & \frac{1}{6}
\end{array}$$

**Algorithm (practical):**

1. Initialize $t=t_0$, $y=y_0$, choose $\mathrm{d} t$.
2. For each step:
   1. Compute $k_1,k_2,k_3,k_4$ as above.
   2. Update $y\leftarrow y + \frac{\mathrm{d} t}{6}(k_1+2k_2+2k_3+k_4)$.
   3. Set $t\leftarrow t+\mathrm{d} t$.
3. Repeat until final time.

This is the classical explicit RK4 ODE solver. It is implemented at [includes/solvers/ODE/rk/explicit/rk4-solver.hpp](../../../includes/solvers/ODE/rk/explicit/rk4-solver.hpp).

### Accuracy and Error

RK4 is a *fourth-order* method. The local truncation error (LTE) per step is $\mathcal{O}(\mathrm{d} t^5)$ and the global truncation error after many steps is $\mathcal{O}(\mathrm{d} t^4)$. The leading contribution to the LTE depends on the fifth time-derivative of the solution; symbolically:

$$ \displaystyle LTE_n = \tilde{y}(t_{n+1}) - y_{n+1} = C\, y^{(5)}(\xi)\, \mathrm{d} t^5 = \mathcal{O}(\mathrm{d} t^5), $$

for some $\xi\in[t_n,t_{n+1}]$ and constant $C$ (the exact constant follows from a full Taylor matching calculation). Summing local errors over $N\approx (t_1-t_0)/\mathrm{d} t$ steps yields the global error

$$ \displaystyle GTE_N = \tilde{y}(t_N) - y_N = \mathcal{O}(\mathrm{d} t^4). $$

**Higher derivatives:** for a scalar ODE $y'=f(t,y)$ the higher derivatives can be written by differentiating along the solution, e.g.

$$ y''(t)=f_t + f_y f, \qquad y^{(3)}(t)=f_{tt} + 2 f_{ty} f + f_{yy} f^2 + f_y y'', $$

and similarly for higher orders (involving higher partial derivatives and tensor contractions for vector-valued states).

### Stability and stiffness (practical note)

Apply RK4 to the linear test equation $y' = \lambda y$. The one-step amplification factor is the degree-4 Taylor polynomial of $e^{z}$ evaluated at $z=\lambda \mathrm{d} t$:

$$ \displaystyle R(z) = 1 + z + \frac{z^2}{2} + \frac{z^3}{6} + \frac{z^4}{24}. $$

Stability requires $|R(z)|<1$. The RK4 stability region is larger than RK1, RK2, and RK3 but still bounded: RK4 is explicit and therefore conditionally stable. For stiff problems (large negative real parts of eigenvalues) implicit methods are preferred.

When using RK4 in practice choose $\mathrm{d} t$ small enough to satisfy stability and accuracy requirements, or use an adaptive-step or implicit solver for stiff systems.

## Example Usage

For the linear test problem

$$ \displaystyle \dot{y}(t) = \alpha y(t), $$

with $y(0)=1.0$, $\alpha=0.9$, $t_0=0.0$, $t_1=20.0$, $\mathrm{d} t=1.0/32.0$, the following snippet shows library usage.

```C++
// The RK4 solver implementation
#include "path/to/includes/solvers/ODE/rk/explicit/rk4-solver.hpp"

// Wrapper: create an ODE function with alpha captured
MyFunc createODEFunction(double alpha)
{
    return [alpha](double t, const dVec& y) -> dVec
    {
        dVec res(y.size());
        for (size_t i = 0; i < y.size(); ++i)
            res[i] = alpha * y[i];
        return res;
    };
}

// Initial condition
dVec y0 = {1.0};

// Create ODE function with alpha = 0.9
MyFunc f = createODEFunction(0.9);

// Method A: use SolverParameters (returns SolverResults)
SolverParameters params;
params.derivative = f;
params.initial_conditions = y0;
params.t0 = 0.0;
params.t1 = 20.0;
params.\mathrm{d} t = 1.0 / 32.0;

SolverResults results = rk4_solver(params);
dMatrix solutions = results.solution;
dVec times = results.time_points;

// Method B: convenience overload (returns dMatrix directly)
dMatrix solutions_alt = rk4_solver(f, y0, 0.0, 20.0, 1.0 / 32.0);

// Analytical solution for comparison: y(t) = y0 * exp(alpha * t)
```

Notes

- RK4 is a good general-purpose explicit integrator for non-stiff problems.
- For problems requiring error control, consider adaptive-step methods (Dormand–Prince, embedded RK pairs) or implicit solvers for stiffness.

<!-- markdownlint-enable MD018 MD051 -->
