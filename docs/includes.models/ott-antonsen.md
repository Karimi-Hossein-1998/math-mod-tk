# Ott–Antonsen Dimensionality Reduction (OA)

This page briefly documents the Ott–Antonsen dimensionality-reduction utilities included under `includes/models/ott-antonsen.dim-red` and related headers (`OA.hpp`). The OA reduction provides low-dimensional descriptions for large populations of phase oscillators under certain assumptions.

## Purpose

The OA ansatz reduces the dynamics of large all-to-all coupled oscillator populations to a closed set of equations for a small number of complex order-parameter variables. The codebase includes helper functions and examples demonstrating the reduction and how to use the reduced equations with the solvers.

## Files and Organization

- `includes/models/OA.hpp` — public header that aggregates OA helpers and exposed wrappers.
- `includes/models/ott-antonsen.dim-red/` — implementation details, helpers and example files (`OA-basic.hpp`, `OA-expanded.hpp`, `OA-helper.hpp`) and auxiliary documentation (`OA.md`, `OA.html`).

## Usage

The OA utilities typically expose functions that return reduced RHS functions compatible with the solver API (i.e. `MyFunc` or similarly typed complex-to-real adapters). Consult the implementation headers for the available reduction variants and example drivers.

## Example and Further Reading

See `includes/models/ott-antonsen.dim-red/OA-basic.hpp` and [`OA.md`](../../includes/models/ott-antonsen.dim-red/OA.md) for worked examples and derivation notes. Use the wrapper pattern from [models.md](models.md) to construct a `MyFunc` you can hand to the solvers.
