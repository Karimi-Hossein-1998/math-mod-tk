# Best Practices for Solving Delay Differential Equations (DDEs)

## Scope and audience

This document explains common, robust approaches used in scientific computing for solving delay differential equations (DDEs). It targets researchers and developers implementing or using DDE solvers and provides practical recipes, numerical considerations, and implementation guidance for constant, time-dependent, and state-dependent delays, including stiff problems and event-driven discontinuities.

## Problem formulation

A standard retarded DDE has the form

$$
\dot x(t) = f\big(t,\;x(t),\;x(t-\tau_1(t,x)),\;x(t-\tau_2(t,x)),\;\dots\big),
$$

with given history for $t\le t_0$:

$$
x(t) = \phi(t),\quad t\le t_0.
$$

- If delays $\tau_i$ are independent of the solution, they are constant or time-dependent delays.
- If any $\tau_i$ depends on the state $x$, the problem is a state-dependent delay (sd-DDE), which is significantly harder.

Define retarded vs. neutral:

- Retarded DDE: delays appear only in arguments of $x(\cdot)$, not in derivatives.
- Neutral DDE: derivatives of delayed terms appear; treat separately (not covered here).

**Key goals:** accuracy, stability (especially for long-time integration), efficient handling of discontinuities, reliable step-size control, and correct treatment of stiffness and state-dependence.

## High-level approaches (when/why)

- Method of Steps (MoS): gold standard for retarded DDEs with known history. Convert the DDE to a sequence of ODE IVPs on successive intervals and use an ODE solver with dense output for delayed states.
  - Best for: general-purpose DDEs with fixed or time-dependent delays, and many practical problems.
- Collocation / Implicit Runge–Kutta (IRK) on the whole interval: preferable for stiff DDEs or when high stability is required.
  - Best for: stiff problems or when implicitness and higher algebraic order for stability are needed.
- Specialized solvers for state-dependent delays: require rootfinding to locate delay arguments and careful handling of discontinuities and consistency conditions.
  - Best for: sd-DDEs; use solvers that support root finding inside the method (e.g., Julia DelayDiffEq or specialized sd-DDE packages).

## Method of Steps — Detailed recipe

**Idea:** march forward in time over intervals where the right-hand side only references known history or previously computed solution segments.

1. Partition time: for constant maximum delay $\tau_{\max}$, a natural step target is to integrate in intervals of length ≤ $\tau_{\max}$ (practical implementations use variable lengths).
2. On each interval $(t_n, t_{n+1}]$ treat the DDE as an ODE IVP:
   $$
   \dot x(t) = F\big(t,x(t)\big) := f\big(t, x(t), H(t)\big)
   $$
   where $H(t)$ collects delayed values $x(t-\tau_i(\cdot))$ evaluated via interpolation from history.
3. Use an adaptive ODE integrator with dense output (continuous extension) to obtain both step values and accurate interpolation for delayed arguments.
4. When the solver requests $x(t_d)$ with $t_d$ inside the integration interval, evaluate using the continuous extension of the current step; if $t_d$ is in the past, use stored piecewise polynomials (history).

Important details:

- Use an integrator with a continuous extension of order matching the integrator (e.g., Dormand–Prince has a 5th-order continuous extension). Continuous extension avoids order loss when evaluating delays.
- When delays jump or cause arguments to cross discontinuities, locate discontinuities and restart the integrator from the discontinuity with proper one-sided history.

Pseudocode (method-of-steps skeleton):

$$
\text{Given } \phi(t)\text{ for }t\le t_0,\;t_{\text{final}}.
$$

- store history segment [t_0-\tau_{\max}, t_0] from $\phi$.
- while current time < t_final:
  - choose next integration horizon (could be until next discontinuity or up to t_final).
  - integrate ODE with adaptive stepper and continuous output:
    - when evaluating delayed argument $x(t-\tau)$, query continuous output/history interpolation.
  - append new segment to history (store nodes + local interpolant).
  - if discontinuity encountered, align step to the discontinuity, apply jump/consistency, then continue.

## Interpolation and history representation

- Represent history with piecewise polynomials (Hermite, cubic splines, or the solver's dense output polynomials). Requirements:
  - Accuracy: interpolation error must be ≤ solver local error tolerance; otherwise order reduction occurs.
  - Continuity class: at minimum, continuous ($C^0$). For high-order methods, a $C^1$ continuous interpolant reduces artifacts.
- Practical choice: use the integrator's dense output polynomials for the recent interval(s) and a compact piecewise cubic Hermite spline (PCHIP) or monotone cubic for older history—this balances accuracy and stability.
- For long runs, prune older history carefully (keep at least $\tau_{\max}$ window).

## Step-size and error control

- Use adaptive local error control with both relative and absolute tolerances per state:
  - Set `tol = rtol * |x| + atol`.
  - For delays and interpolation, ensure interpolation error is consistent with `tol`.
- Monitor special quantities:
  - If evaluating $x(t-\tau)$ triggers extrapolation (solver asked for $t_d$ outside stored domain), halt and enlarge history or reduce step.
- In presence of discontinuities, reduce step to align exactly to discontinuity boundaries to avoid spurious Gibbs-like behavior.

## Handling discontinuities and events

- Discontinuities occur at:
  - initial time `t_0` due to history mismatch,
  - times where `t - τ_i(t, x)` crosses an event time,
  - state-dependent events (e.g., switches).
- Best practice:
  - Detect event times using rootfinding on event functions; stop at event time, apply jump/consistency, restart method-of-steps using updated history.
  - For events induced by delayed arguments, locate root of `t - τ_i(t,x) - t_event = 0` using continuous output and Newton/Bisection with bracketed intervals.
- When an event is found inside a step, roll back to the event time (or use smaller step) and restart the integrator on both sides.

## State-dependent delays (sd-DDEs)

- sd-DDEs require solving implicit equations to evaluate delayed times: find $s$ such that $s = t - \tau(t, x(t), x(s))$ or variants.
- Strategy:
  - For each evaluation of a delayed argument, solve the scalar implicit equation for the delay argument using robust rootfinding (fused with interpolation of past solution).
  - Ensure consistency and bracketed roots; use safeguarded Newton or Brent's method with good initial guesses from previous steps.
  - Because delay arguments depend on solution, solver must handle discontinuities introduced when roots cross or emerge—restart and re-evaluate events frequently.
- Use dedicated sd-DDE solvers when possible (they implement these mechanics and Jacobian coupling).

## Stiff DDEs and implicit methods

- Stiffness can appear due to fast decay modes or when the delay introduces eigenvalues with large negative real parts.
- For stiff problems:
  - Use implicit collocation / IRK or backward differentiation formulas adapted to DDEs.
  - Implicit collocation allows global polynomial approximations and naturally handles implicit couplings due to delays (but increases algebraic complexity).
  - Practical solvers: RADAR5 (stiff DDEs), and collocation-based implementations in advanced libraries.
- When using implicit methods, provide Jacobian information (see below) or use efficient Jacobian approximations (Broyden or limited-memory updates).

## Jacobians and sensitivities

- Supplying analytic Jacobians improves performance and robustness for stiff/implicit solvers and for sensitivity analysis.
- Structure:
  - Partial derivative w.r.t. current state: $\partial f/\partial x(t)$.
  - Partial derivative w.r.t. delayed state(s): $\partial f/\partial x(t-\tau)$.
- For sd-DDEs, include derivatives of delays $\partial \tau/\partial x$ when forming rootfinding Jacobians.
- Sensitivity equations (forward or adjoint) require propagation including contributions from delayed states; use solver-supported sensitivity modules when available.

## Verification, testing and validation

- Use manufactured solutions to test order of convergence: choose $\phi(t)$ and $f$ so that exact $x(t)$ is known; verify global order by refining tolerances/step sizes.
- Test:
  - Continuity and consistency at initial time.
  - Response to events (discontinuities).
  - Conservation laws or invariants where applicable.
  - Long-time behavior: verify stability and correctness of attractors or periodic solutions.
- Include unit tests that check interpolation accuracy, event detection, and restart mechanics.

## Performance and practical tips

- Choose tolerances carefully: too tight increases cost; too loose creates incorrect solutions—start with `rtol=1e-6`, `atol=1e-9` and adapt to problem scale.
- Cache interpolants and reuse them until the next append to history; avoid rebuilding global interpolants every step.
- Use vectorized evaluation of `f` where possible.
- For large-scale problems, exploit sparsity in Jacobians and use Krylov linear solvers for implicit stages.
- Avoid extrapolation: enforce that delayed query times always lie within stored history; if not, reduce step or extend history.

## Software recommendations

- MATLAB: `dde23` (Runge–Kutta — nonstiff), `ddesd` (state-dependent delays), DDE-BIFTOOL (bifurcation analysis).
- Julia: DelayDiffEq (part of DifferentialEquations.jl) — modern, supports sd-DDEs, stiff solvers, sensitivities.
- For stiff retarded DDEs: RADAR5 (legacy, robust), collocation packages.
- Python: fewer mature options; `py_delay` / `ddeint` exist but are limited—prefer calling Julia or MATLAB for production work or use wrappers.
- For bifurcation and continuation: DDE-BIFTOOL is the standard.

## Example: adaptive RK + continuous-extension (compact pseudocode)

1. Initialize `history` from `phi`.
2. While `t < t_final`:
   - predict `t_next = min(t + H, t_final)` (H chosen adaptively).
   - Integrate ODE IVP on `[t, t_next]` with adaptive RK and dense output.
   - When solver evaluates `x(t - tau)`:
     - if `t - tau >= history.start`, evaluate using history interpolant.
     - else error or extend history.
   - If event/discontinuity detected in `[t, t_next]`:
     - locate `t_event`, roll back to `t_event`, apply jump/consistency, and restart step.
   - Append dense output polynomial + nodes to `history` and advance `t`.

## Debugging checklist

- Check history consistency at `t0`: ensure $\phi(t_0) = \lim_{t\downarrow t_0} x(t)$ if required.
- Visualize delayed arguments: plot $t-\tau_i(t,x(t))$ to see crossing with discontinuities.
- Turn on diagnostic logging for interpolation queries and rootfinding iterations.
- Verify that the solver never needs to extrapolate history.

## Summary recommendations (short)

- Use the method of steps with an adaptive ODE solver that provides a continuous extension for most retarded DDEs.
- For stiff problems, prefer implicit collocation/IRK or stiff DDE solvers.
- For state-dependent delays, use specialized sd-DDE solvers and robust rootfinding for delayed arguments.
- Treat discontinuities as first-class events: detect, align, restart.
- Provide Jacobians and exploit problem structure for performance.
- Verify with manufactured solutions, and use tested libraries (MATLAB `dde23`/`ddesd`, Julia DelayDiffEq) where possible.

## References and further reading

- Baker, Paul, and N. Shampine. "Solving delay differential equations with MATLAB." (detailed discussion of `dde23` concepts).
- Hairer, Nørsett, and Wanner. "Solving Ordinary Differential Equations II" — chapter on DDEs and collocation.
- Engelborghs et al. "DDE-BIFTOOL v. 2.00: a MATLAB package for bifurcation analysis of delay differential equations".
- DifferentialEquations.jl documentation (DelayDiffEq) — modern implementations and examples.
