# Models (`models/`)

This directory contains implementations of the mathematical models used by the library, plus lightweight wrappers that adapt model code to the solver interfaces.

## Table of Contents

1. [Models (`models/`)](#models-models)
    1. [Table of Contents](#table-of-contents)
    2. [Overview](#overview)
    3. [Phase Oscillator Models](#phase-oscillator-models)
    4. [Bottom Line](#bottom-line)

## Overview

The `models/` module hosts model implementations (for example the Kuramoto family of phase-oscillator models) and small adapter/wrapper functions that produce the `MyFunc` signature expected by the solvers:

```text
MyFunc := std::function<dVec(double, const dVec&)> // (t, y) -> dy/dt
```

Models in this directory are intended to be self-contained and documented; where a model requires auxiliary parameters (coupling matrix, natural frequencies, etc.) a `params` structure and a `*_wrapper` producing a `MyFunc` should be provided.

The following pages provide more detail on the provided models and recommended wrapper patterns.

## Phase Oscillator Models

- **Kuramot Family:** See [kuramoto.md](./kuramoto.md) for the Kuramoto model implementation and wrapper pattern documentations.
- **Ott-Antonsen Dimensional Reduction:** See [ott-antonsen.md](./ott-antonsen.md) for the Ott–Antonsen dimensionality-reduction utilities documentation.

## Bottom Line

If you add any models (family of models), please follow the example of the [Phase Oscillator Models](#phase-oscillator-models) in terms of implementation and documentation guidelines.

See also: the implementation headers in `includes/models` and the solver documentation in docs/... .
