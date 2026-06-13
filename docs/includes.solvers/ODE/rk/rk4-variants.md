<!-- markdownlint-disable MD018 MD051 -->
# Runge-Kutta 4 Variants

In addition to the classical RK4 method, several variants exist that offer different trade-offs in accuracy, stability, and computational efficiency. This document covers three important RK4 variants: the 3/8 Rule, Ralston's method, and Gill's method. All are fourth-order explicit methods but with different Butcher tableaus and coefficients.

## Table of Contents

1. [Runge-Kutta 4 Variants](#runge-kutta-4-variants)
    1. [Table of Contents](#table-of-contents)
    2. [3/8 Rule (Simpson's 3/8 Rule)](#38-rule-simpsons-38-rule)
    3. [Ralston's RK4](#ralstons-rk4)
    4. [Gill's RK4](#gills-rk4)
    5. [Comparison and Usage](#comparison-and-usage)
        1. [Example Usage (All Variants)](#example-usage-all-variants)

## 3/8 Rule (Simpson's 3/8 Rule)

The 3/8 Rule RK4 is an alternative fourth-order method based on Simpson's 3/8 quadrature rule. It uses stage points at $t = 0, \frac{dt}{3}, \frac{2dt}{3}, dt$ instead of the classical $0, \frac{dt}{2}, \frac{dt}{2}, dt$ distribution.

**Derivation:**

The method is constructed by matching the Taylor series expansion to fourth order using the RK stage framework with a different set of stage nodes (Runge-Kutta abscissae). The choice of nodes $c_i = [0, 1/3, 2/3, 1]$ and the corresponding weights $b_i = [1/8, 3/8, 3/8, 1/8]$ come from requiring consistency with the Simpson 3/8 quadrature rule applied to numerical integration.

**Butcher tableau:**

$$\begin{array}{c|cccc}
0 & & & & \\
\frac{1}{3} & \frac{1}{3} & & & \\
\frac{2}{3} & -\frac{1}{3} & 1 & & \\
1 & 1 & -1 & 1 & \\
\hline
& \frac{1}{8} & \frac{3}{8} & \frac{3}{8} & \frac{1}{8}
\end{array}$$

**Update formula:**

$$\displaystyle k_1 = f(t_n, y_n), \quad k_2 = f\left(t_n + \frac{dt}{3}, y_n + \frac{dt}{3}k_1\right),$$

$$\displaystyle k_3 = f\left(t_n + \frac{2dt}{3}, y_n - \frac{dt}{3}k_1 + dt\, k_2\right), \quad k_4 = f(t_n+dt, y_n + dt(k_1 - k_2 + k_3)),$$

$$\displaystyle y_{n+1} = y_n + \frac{dt}{8}(k_1 + 3k_2 + 3k_3 + k_4).$$

**Characteristics:**

- Fourth-order accurate ($\mathcal{O}(dt^5)$ LTE, $\mathcal{O}(dt^4)$ GTE).
- Different stability region from classical RK4; sometimes slightly larger stability domain for certain problems.
- No significant computational advantage over classical RK4, but can be useful for problems where the Simpson rule weighting is natural.

**Usage:** [includes/solvers/ODE/rk/explicit/rk4-variants.hpp](../../../includes/solvers/ODE/rk/explicit/rk4-variants.hpp) provides `rk4_38_solver(params)` and the overload `rk4_38_solver(f, y0, t0, t1, dt)`.

---

## Ralston's RK4

Ralston's method is a fourth-order variant optimized to minimize the error bound by careful choice of the stage coefficients. It employs irrational coefficients involving $\sqrt{5}$ to achieve optimal error reduction.

**Derivation:**

Ralston (1962) derived optimal RK4 coefficients by minimizing the error norm over the stability region. The resulting method uses stage points and weights that differ significantly from the classical method. The optimization involved solving a system of quadrature and Runge-Kutta order conditions to minimize a specific error functional.

**Key coefficients (involving $\sqrt{5}$):**

$$c_2 = \frac{2}{5}, \quad c_3 = \frac{\sqrt{5}-1}{4}, \quad c_4 = 1.$$

The weights and stage coefficients are:

$$\displaystyle a_{21} = \frac{2}{5}, \quad a_{32} = \frac{25-13\sqrt{5}}{40}, \quad a_{31} = \frac{11-\sqrt{5}}{40},$$

$$\displaystyle a_{42} = \frac{10-7\sqrt{5}}{40}, \quad a_{41} = \frac{5-\sqrt{5}}{40}, \quad a_{43} = \frac{5+\sqrt{5}}{4}.$$

**Butcher tableau:**

$$\begin{array}{c|cccc}
0 & & & & \\
\frac{2}{5} & \frac{2}{5} & & & \\
\frac{\sqrt{5}-1}{4} & \frac{11-\sqrt{5}}{40} & \frac{25-13\sqrt{5}}{40} & & \\
1 & \frac{5-\sqrt{5}}{40} & \frac{10-7\sqrt{5}}{40} & \frac{5+\sqrt{5}}{4} & \\
\hline
& \frac{5-\sqrt{5}}{20} & \frac{5+3\sqrt{5}}{20} & \frac{5+3\sqrt{5}}{20} & \frac{5-\sqrt{5}}{20}
\end{array}$$

(Note: Simplified row sums; weights and intermediate coefficients are symmetric pairs.)

**Characteristics:**

- Fourth-order accurate with optimized error constant.
- Irrational coefficients (involving $\sqrt{5}$) require floating-point evaluation but are computed once per step.
- Slightly smaller error bound than classical RK4 for the same step size.
- Marginally different stability region.

**Usage:** Call `rk4_ralston_solver(params)` or `rk4_ralston_solver(f, y0, t0, t1, dt)` from [includes/solvers/ODE/rk/explicit/rk4-variants.hpp](../../../includes/solvers/ODE/rk/explicit/rk4-variants.hpp).

---

## Gill's RK4

Gill's method is a memory-efficient fourth-order RK4 variant designed to reduce the number of temporary storage registers needed. It reuses vector temporary storage by carefully ordering the stage calculations, making it attractive for problems with large state vectors or memory-constrained systems.

**Derivation:**

Gill (1951) reformulated the RK4 update to minimize intermediate storage by noting that the intermediate vectors $y_{\text{temp}}$ need not all be stored simultaneously. The method rearranges the calculation order and uses coefficients involving $\sqrt{2}$ to achieve a specific factorization that reduces storage from 4 temporary vectors (classical RK4) to effectively 1 with careful reuse.

**Key coefficients (involving $\sqrt{2}$):**

Define $\alpha = 1 - \frac{1}{\sqrt{2}}$, $\beta = 1 + \frac{1}{\sqrt{2}}$. Then:

$$c_1 = 0, \quad c_2 = c_3 = \frac{1}{2}, \quad c_4 = 1.$$

Intermediate update coefficients involve $\alpha$, $\beta$, and $\frac{1}{\sqrt{2}}$ to permit storage reuse.

**Butcher tableau:**

$$\begin{array}{c|cccc}
0 & & & & \\
\frac{1}{2} & \frac{1}{2} & & & \\
\frac{1}{2} & \frac{\sqrt{2}-1}{2\sqrt{2}} & \frac{\sqrt{2}-1}{\sqrt{2}} & & \\
1 & 0 & -\frac{1}{\sqrt{2}} & 1+\frac{1}{\sqrt{2}} & \\
\hline
& \frac{1}{6} & \frac{2-\sqrt{2}}{6} & \frac{2+\sqrt{2}}{6} & \frac{1}{6}
\end{array}$$

(Slightly simplified; exact coefficients are functions of $\sqrt{2}$.)

**Characteristics:**

- Fourth-order accurate ($\mathcal{O}(dt^5)$ LTE, $\mathcal{O}(dt^4)$ GTE).
- Memory-efficient: uses only one temporary vector instead of multiple intermediates.
- Slightly different error constant and stability region due to the reordered calculation.
- Useful for large systems where memory bandwidth or cache efficiency is a bottleneck.

**Usage:** Call `rk4_gill_solver(params)` or `rk4_gill_solver(f, y0, t0, t1, dt)` from [includes/solvers/ODE/rk/explicit/rk4-variants.hpp](../../../includes/solvers/ODE/rk/explicit/rk4-variants.hpp).

---

## Comparison and Usage

All four RK4 variants (classical, 3/8 Rule, Ralston, Gill) are fourth-order methods with $\mathcal{O}(dt^5)$ local and $\mathcal{O}(dt^4)$ global errors. Their differences lie in the error coefficient, stability region, and computational cost:

| Variant | Error Constant | Memory | Stability | Notes |
|---------|----------------|--------|-----------|-------|
| Classical | Standard | 3–4 vectors | Well-known | Most widely used |
| 3/8 Rule | Standard | 3–4 vectors | Slightly different | Simpson-based weighting |
| Ralston | Optimized (smaller) | 3–4 vectors | Optimized | Best error for given $dt$ |
| Gill | Standard | 1 vector + reuse | Similar to classical | Efficient for large $m$ |

**Recommendations:**

- **Classical RK4:** Default choice for most problems. Simple, well-tested, stable.
- **3/8 Rule:** Use if Simpson quadrature weighting is natural for the problem, or if you need a different stability region.
- **Ralston's RK4:** Choose when you want the tightest error bound and are willing to compute $\sqrt{5}$-based coefficients.
- **Gill's RK4:** Prefer for large-dimensional systems (e.g., $m \gg 1000$) where memory bandwidth or cache line efficiency is critical.

For most problems, **classical RK4** is sufficient. Choose a variant only if you have a specific reason (e.g., memory constraints, stability requirements, or proven error reduction benefits for your application).

### Example Usage (All Variants)

```C++
#include "path/to/includes/solvers/ODE/rk/explicit/rk4-variants.hpp"

// ODE function
MyFunc f = [](double t, const dVec& y) -> dVec { /* ... */ };
dVec y0 = {1.0};
double t0 = 0.0, t1 = 20.0, dt = 0.01;

// Classical RK4
SolverResults res_classical = rk4_solver(SolverParameters{...});

// 3/8 Rule
SolverResults res_38 = rk4_38_solver(SolverParameters{...});

// Ralston's RK4
SolverResults res_ralston = rk4_ralston_solver(SolverParameters{...});

// Gill's RK4
SolverResults res_gill = rk4_gill_solver(SolverParameters{...});
```

All return `SolverResults` with `.solution` (dMatrix) and `.time_points` (dVec) fields.

<!-- markdownlint-enable MD018 MD051 -->
