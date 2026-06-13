# Kuramoto Model

This document describes the Kuramoto model implementation included in `includes/models/kuramoto` and shows the recommended wrapper pattern for using the model with the solver APIs.

## Table of Contents

1. [Kuramoto Model](#kuramoto-model)
    1. [Table of Contents](#table-of-contents)
    2. [Model](#model)
    3. [Wrapper Pattern](#wrapper-pattern)
    4. [Usage Example](#usage-example)
    5. [Notes and Recommendations](#notes-and-recommendations)

## Model

The generalized Kuramoto model implemented in the code has the signature:

```cpp
inline dVec kuramoto_general(
    double time,
    const dVec& theta,
    const dVec& omega,
    double K,
    const dMatrix& adj,
    double alpha
)
```

Where:

- `theta` are the oscillator phases
- `omega` are intrinsic frequencies
- `K` is coupling strength
- `adj` is the adjacency/coupling matrix
- `alpha` is the phase-lag parameter

The implementation computes the phase derivatives for each oscillator and returns a `dVec` of size `N` (size of the system).

## Wrapper Pattern

To use the model with the solvers (which expect a `MyFunc`), provide a small params struct and a wrapper that captures parameters by value:

```cpp
struct KuramotoParams {
    double K;
    int N;
    dVec omega;
    dMatrix adj;
    double alpha;
};

inline MyFunc kuramoto_general_wrapper(const KuramotoParams& params)
{
    return [params](double t, const dVec& theta) -> dVec
    {
        return kuramoto_general(t, theta, params.omega, params.K, params.adj, params.alpha);
    };
}
```

## Usage Example

Create the params, build the wrapper, then pass the resulting `MyFunc` to a solver (example uses the RK4 solver):

```cpp
KuramotoParams params(100);
params.K = 1.0;
// fill params.omega and params.adj as required

MyFunc model = kuramoto_general_wrapper(params);

SolverParameters p;
p.dt = 0.01;
p.derivative = model;
// set other solver parameters (t0, t1, initial_conditions)

auto res = rk4_solver(p);
```

## Notes and Recommendations

- Use `random_uniform` from the initializers to create reproducible initial conditions. (You can also use other initializers based on your particular needs, e.g. `splay`, the key is to use a seed to ensure reproducibility.)
- When simulating large networks, prefer sparse or specialized adjacency storage and exploit structure to speed up coupling computations. Also feel free to write your own specialized model if necessary.

See [models.md](models.md) for the module overview and ... for solver selection guidance.
