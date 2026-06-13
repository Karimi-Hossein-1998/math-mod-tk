# Network Topology Utilities ([`topology.hpp`](../../includes/network/topology.hpp))

This module provides a comprehensive set of functions for generating, manipulating, and analyzing network topologies. These utilities are essential for simulating systems of coupled oscillators, multiplex networks, and other graph-based models in mathematical modeling and computational science.

## Table of Contents

1. [Network Topology Utilities (`topology.hpp`)](#network-topology-utilities-topologyhpp)
    1. [Table of Contents](#table-of-contents)
    2. [Overview](#overview)
    3. [dMatrix Conversion \& Analysis](#dmatrix-conversion--analysis)
    4. [Basic Topology Generators](#basic-topology-generators)
    5. [Erdős–Rényi Networks](#erdősrényi-networks)
    6. [Small-World Networks (Watts–Strogatz)](#small-world-networks-wattsstrogatz)
    7. [Structured Networks](#structured-networks)
    8. [Multiplex Networks](#multiplex-networks)
    9. [Unified Topology Interface](#unified-topology-interface)
    10. [Degree Calculations](#degree-calculations)
    11. [Usage Notes](#usage-notes)

## Overview

The functions in `topology.hpp` allow you to:

- Generate random, structured, and multiplex network topologies
- Convert between dense and sparse matrix representations
- Analyze network properties such as degree distributions and sparsity
- Compose multi-layer networks and block structures
- Select and parameterize topologies via a unified interface

All functions are header-only and designed for performance, flexibility, and ease of use.

## dMatrix Conversion & Analysis

- `dense_to_sparse(const dMatrix&)` — Converts a dense adjacency matrix to a sparse format for memory efficiency.
- `density(const dMatrix&, double threshold=1e-12)` — Computes the fraction of nonzero off-diagonal entries (sparsity density).
- `dense_to_sparse_conditional(const dMatrix&, SparsedMatrix&, double density_threshold=0.5, double zero_threshold=1e-12)` — Converts to sparse only if density is below a threshold; otherwise returns density.

**Example:**

```C++
dMatrix dense_form = {{0.0, 1.0, 0.0},
                       {0.0, 0.0, 0.0},
                       {2.0, 0.0, 0.0}};
SparsedMatrix sparse_form = dense_to_sparse(dense_form);
double dens = density(dense_form); // dens = 0.222...
```

## Basic Topology Generators

- `random(N, min_weight, max_weight, seed)` — Generates a random network with edge weights uniformly distributed in `[min_weight, max_weight]`.
- `random_symmetric(N, min_weight, max_weight, seed)` — Generates a symmetric random network.

**Example:**

```C++

dMatrix rand_net = random(100, 0.0, 1.0, 42); // a random network connecting 100 nodes with connection weights (strength) in [0, 1) range
```

## Erdős–Rényi Networks

- `erdos_renyi(N, p, min_weight, max_weight, seed)` — Standard random graph with connection probability $p$.
- `erdos_renyi_uniform(N, p, min_weight, max_weight, seed)` — Erdős–Rényi with exactly $N(N-1)p$ edges.
- `erdos_renyi_symmetric(N, p, min_weight, max_weight, seed)` — Symmetric Erdős–Rényi network.
- `erdos_renyi_symmetric_uniform(N, p, min_weight, max_weight, seed)` — Symmetric with exactly $N(N-1)p/2$ edges.

**Example:**

```C++
dMatrix er_net = erdos_renyi(100, 0.1, 0.0, 1.0, 123); // an Erdős–Rényi network of 100 nodes with connection probability 0.1 and weights in [0, 1)

dMatrix er_sym_net = erdos_renyi_symmetric(100, 0.1, 0.0, 1.0, 123); // symmetric version

dMatrix er_uniform_net = erdos_renyi_uniform(100, 0.1, 0.0, 1.0, 123); // uniform edge count version

dMatrix er_sym_uniform_net = erdos_renyi_symmetric_uniform(100, 0.1, 0.0, 1.0, 123); // symmetric uniform edge count version

```

## Small-World Networks (Watts–Strogatz)

- `small_world(N, k, beta, weight, seed)` — Generates a ring lattice with $k$ neighbors per node and rewiring probability $\beta$.
- `small_world_sparse(N, k, beta, weight, seed)` — Returns a sparse-format small-world network.
- `small_world_directed(N, k, beta, weight, seed)` — Directed small-world network.

**Example:**

```C++
dMatrix sw_net = small_world(100, 4, 0.1, 1.0, 42); // small-world network with 100 nodes, each connected to 4 neighbors, rewiring probability 0.1, uniform weight 1.0

SparsedMatrix sw_sparse_net = small_world_sparse(100, 4, 0.1, 1.0, 42); // sparse version

dMatrix sw_dir_net = small_world_directed(100, 4, 0.1, 1.0, 42); // directed small-world network

```

## Structured Networks

- `multilayered(const std::vector<dMatrix>& layers)` — Composes a block-diagonal matrix from multiple layers.
- `modular(module_size, num_modules, p_in, p_out, in_weight, out_weight, seed)` — Modular network with dense intra-module and sparse inter-module connections.
- `hierarchical(N, levels, p_in, p_out, in_weight, out_weight, level_decay, seed, base_module_num)` — Recursively nested modules with level-dependent connection probabilities.

**Example:**

```C++
dMatrix mod_net = modular(10, 5, 0.8, 0.1, 1.0, 0.1, 42); // modular network with 5 modules of size 10, intra-module connection probability 0.8, inter-module 0.1

dMatrix hier_net = hierarchical(100, 3, 0.8, 0.1, 1.0, 0.1, 0.5, 42, 10); // hierarchical network with 3 levels, base module size 10

```

## Multiplex Networks

- `effective_multiplex(const std::vector<dMatrix>& layers, const dVec& layer_weights)` — Computes a weighted sum of multiple network layers.
- `generate_multiplex_network_layers(N, num_layers, layer_generation_types, layer_params, layer_seeds)` — Generates multiple layers for a multiplex network, each with its own topology and parameters.

## Unified Topology Interface

- `pick_topology(topology_type, N, a, b, c, seed, d, e, f, base_module_num)` — Creates a network topology by name, dispatching to the appropriate generator with parameters. Supported types include:

- "random"
- "erdos_renyi"
- "erdos_renyi_uniform"
- "erdos_renyi_symmetric"
- "erdos_renyi_symmetric_uniform"
- "small_world"
- "small_world_directed"
- "modular"
- "hierarchical"

## Degree Calculations

- `in_degrees(const dMatrix&, double threshold=1e-12)` / `in_degrees(const SparsedMatrix&, double threshold=1e-12)` — Returns the in-degree (number of incoming edges) for each node.
- `out_degrees(const dMatrix&, double threshold=1e-12)` / `out_degrees(const SparsedMatrix&, double threshold=1e-12)` — Returns the out-degree (number of outgoing edges) for each node.

**Note:** Degrees are computed based on edges with weights above the specified threshold.

The formulae are:

$$ \displaystyle \text{in\_degree}(i) = \sum_{j=1}^{N} \mathbf{1}_{\{A_{ij} > \text{threshold}\}} $$

,

$$ \displaystyle \text{out\_degree}(i) = \sum_{j=1}^{N} \mathbf{1}_{\{A_{ji} > \text{threshold}\}} $$

## Usage Notes

- All functions perform error checking and throw `std::invalid_argument` for invalid parameters.
- For large or sparse networks, prefer sparse matrix representations for memory and speed.
- Use the unified `pick_topology` interface for flexible topology selection in simulations.
- For multiplex networks, ensure all layers are square and of the same size.
- See comments in `topology.hpp` for parameter details and advanced usage.

For further details, see the comments in each function in [`includes/network/topology.hpp`](../../includes/network/topology.hpp) and the [network section of the README](../../README.md#network).
