# DDE Solvers (constant delay)

For the purpose of solving *DDEs* one approach is to modify the existing *ODE* solvers and re-use them to solve the *DDEs*.

Assume the system given as:

$$ \displaystyle y' = f\left(t,y\left(t\right),y\left(t-\tau\right)\right)\, y(t) = \phi(t) \text{for} t\in [t_0-\tau,t_0].$$

This is called a *Delay Differantial Equation* with single constant delay. $\phi(t)$ is called the history function (it resembles the initial condition, as if the initial condition is now *infinite-dimensional*.) and it provides the values of the state variable $y$ within the range of $[t_0-\tau,t_0]$ of the independent variable $t$.

## Table of Contents

1. [DDE Solvers (constant delay)](#dde-solvers-constant-delay)
    1. [Table of Contents](#table-of-contents)
    2. [Modification](#modification)
    3. [Example and Usage](#example-and-usage)

## Modification

Imagine the *Adams-Bashforth/Adams-Moulton* methods for solving *ODEs* (which were discussed in [multistep solvers for *ODEs* documentations](../ODE/multistep/multi-step.md)). To modify these methods for solving *DDEs*, we need to account for the delay term $y(t-\tau)$ in the function $f$. This involves:

1. **Storing Past Values**: We need to maintain a history of past values of $y$ for $t\in [t_0-tau,t_0)$ to compute $y(t-\tau)$ when needed. This can be done using a data structure that allows us to efficiently access past values based on the current time step. Hence we compute and store the values of $y$ at discrete time points, and when we need to compute $y(t-\tau)$, we can interpolate between the stored values if $t-\tau$ does not correspond to a stored time point. (As of now only linear interpolation is implemented for this purpose, but higher-order interpolation methods can be implemented in the future.)
2. **Interpolation**: When the delay $\tau$ does not align with the discrete time steps, we need to interpolate the value of $y(t-\tau)$ from the stored history. This can be done using linear interpolation or higher-order interpolation methods depending on the desired accuracy.
3. **Bootstrap**: Since the method is multistep, we need to compute the first few values of $y$ for $t\geq t_0$ using a different method (like Runge-Kutta) to "bootstrap" the multistep method before we can start using the Adams-Bashforth/Adams-Moulton methods for the main integration.
4. **$t_0$**: When we start the integration at $t_0$, we already have the history function $\phi(t)$ defined for $t\in [t_0-\tau,t_0]$. This means that we can directly use $\phi(t)$ to compute $y(t-\tau)$ for $t$ in the initial steps of the integration, without needing to rely on stored values from previous time steps. This allows us to start the integration immediately at $t_0$ without needing to wait for a few steps to build up the history.
5. **Multiple Constant Delays**: If there are multiple constant delays, we need to store the history for $t \in [t_0-tau_{\text{max}},t0)$ and interpolate values for each delay term separately. This adds complexity to the implementation, as we need to interpolate multiple times across the history.

This is the modification that has been implemented here. Both for *Adams-Bashforth* and *Adams-Bashforth-Moulton* (PECE...) solvers.

## Example and Usage

There are four functions (for two solvers with two overloads.)

1. `adams_bashforth_dde`: This function implements the Adams-Bashforth method for solving DDEs. It takes the delay function, history function, time span, initial condition, step size, and other parameters to compute the solution of the DDE.
2. `adams_bashforth_moulton_dde`: This function implements the Adams-Bashforth-Moulton (PECE) method for solving DDEs. It also takes similar parameters as the Adams-Bashforth function but includes additional parameters for controlling the corrector iterations (and tolerance in future).
