# Mathematical Modelling Framework

THis is a project for a header-only C++ mathematical modelling framework focused on mathematical accuracy, performance and speed, fair ease of use, and modern C++ standards (and occasionally some non-standard but widely supported dependencies).

It is made in two main parts:

- `.\includes` - The main library headers (actual models, solvers, utilities, etc.)
- `.\Examples` - Example use-cases and applications of the library. Even some works similar to my M.Sc. thesis.

## The Library (`.\includes`)

The library is something like this

```text
|--includes
    |    |--initializers
    |    |    |--initials.hpp
    |    |--interpolators
    |    |    |--lagrange-interpolator.hpp
    |    |    |--linear-interpolator.hpp
    |    |--linalg
    |    |    |--basics
    |    |    |    |--decomposers.hpp
    |    |    |    |--eigen.hpp
    |    |    |    |--matrix-operations.hpp
    |    |    |    |--matrix-properties.hpp
    |    |    |--linsyssolvers
    |    |         |--banded-solver.hpp
    |    |         |--gaussian-elimination.hpp
    |    |         |--ldlt-solve.hpp
    |    |         |--llt-solve.hpp
    |    |         |--lu-solve.hpp
    |    |         |--qr-solve.hpp
    |    |         |--svd-solve.hpp 
    |    |         |--tridiagonal-solver.hpp
    |    |--miscellany
    |    |    |--ABM-Coefs
    |    |    |    |--abm_coefs_order1.txt
    |    |    |    |--abm_coefs_order2.txt
    |    |    |    |--abm_coefs_order3.txt
    |    |    |    |--abm_coefs_order4.txt
    |    |    |    |--abm_coefs_order5.txt
    |    |    |    |--abm_coefs_order6.txt
    |    |    |    |--abm_coefs_order7.txt
    |    |    |    |--abm_coefs_order8.txt
    |    |    |    |--abm_coefs_order9.txt
    |    |    |    |--abm_coefs_order10.txt
    |    |    |--abm-v0.0.py
    |    |    |--abm.py
    |    |--models
    |    |    |--kuramoto
    |    |    |    |--general.hpp
    |    |    |    |--order.hpp
    |    |    |    |--sparse.hpp
    |    |    |    |--special.hpp
    |    |    |--ott-antonsen.dim-red
    |    |    |    |--OA-basic.hpp
    |    |    |    |--OA-expanded.hpp
    |    |    |    |--OA-helper.hpp
    |    |    |    |--OA-mh.md
    |    |    |    |--OA.html
    |    |    |    |--OA.md
    |    |    |--kuramoto.hpp
    |    |    |--models.phase-oscillators.hpp
    |    |    |--OA.hpp
    |    |--namespaces
    |    |    |--am-coefs.hpp
    |    |--network
    |    |    |--topology.hpp
    |    |--solvers
    |    |    |--DDE
    |    |    |    |--adaptive
    |    |    |    |    |--adaptive-dde-common.hpp
    |    |    |    |    |--adaptive-dde-custom.hpp
    |    |    |    |    |--adaptive-dde-general-common.hpp
    |    |    |    |    |--adaptive-dde-general-custom.hpp
    |    |    |    |--multistep
    |    |    |    |--rk
    |    |    |         |--explicilt
    |    |    |              |--rk1-dde-general-solver.hpp
    |    |    |              |--rk1-dde-solver.hpp
    |    |    |              |--rk2-dde-general-solver.hpp
    |    |    |              |--rk2-dde-solver.hpp
    |    |    |              |--rk3-dde-general-solver.hpp
    |    |    |              |--rk3-dde-solver.hpp
    |    |    |              |--rk4-dde-general-solver.hpp
    |    |    |              |--rk4-dde-general-variant.hpp
    |    |    |              |--rk4-dde-solver.hpp
    |    |    |              |--rk4-dde-variants.hpp
    |    |    |--miscellany
    |    |    |    |--validator.hpp     
    |    |    |--ODE
    |    |    |    |--adaptive
    |    |    |    |    |--adaptive-common.hpp
    |    |    |    |    |--adaptive-custom.hpp
    |    |    |    |--multistep
    |    |    |    |    |--ab-solver.hpp
    |    |    |    |    |--ab-split.hpp
    |    |    |    |    |--abm-solver.hpp
    |    |    |    |--rk
    |    |    |         |--explicilt
    |    |    |              |--rk1-solver.hpp
    |    |    |              |--rk2-solver.hpp
    |    |    |              |--rk3-solver.hpp
    |    |    |              |--rk4-solver.hpp
    |    |    |              |--rk4-variants.hpp
    |    |    |--dde-solvers.hpp
    |    |    |--ode-solvers.hpp
    |    |    |--solver.hpp
    |    |--typedefs
    |    |    |--complex.hpp
    |    |    |--header.hpp
    |    |--utility
    |         |--adaptive-solvers-utility.hpp
    |         |--dde-utility.hpp
    |         |--print.hpp
    |         |--utility.hpp
    |         |--write.hpp
    |---mm.hpp
```

and the whole project tree is:

```text
|--Examples
|--includes
|--LICENSE
```
