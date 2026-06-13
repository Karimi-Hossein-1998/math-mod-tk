# Initializers (`initials.hpp`)

This directory is for any functions and headers that help initialize the system state or parameters before running simulations. This may include functions to set initial conditions, randomize parameters, or prepare any necessary data structure.

Here in this file we will fully explain all the functions that have been implemented within **`initials.hpp`**.

## Functions

1. **Random Uniform Initialization**

   **Function Name:** `random_uniform`

   **Description:** This function initializes the phases of oscillators randomly with a uniform distribution between specified minimum and maximum values.

   **Parameters:**
   - `size_t N`: The number of oscillators to initialize.
   - `double min_val`: The minimum value for the uniform distribution (typically 0).
   - `double max_val`: The maximum value for the uniform distribution (typically 2π).
   - `unsigned int seed` (optional): Seed for the random number generator to ensure reproducibility.

   **Returns:** A vector of size `N` containing the initialized phases.

    **Implementation:**

    ```C++
    // Uniform distribution in [min, max)
    inline dVec  random_uniform(
        size_t   N,
        double   min_val,
        double   max_val,
        unsigned seed
    )
    {
        // Error Handling
        if (N == 0) 
        {
            throw std::invalid_argument("[random_uniform_phases] Number of phases N cannot be zero.");
        }
        if (min_val > max_val) 
        {
            throw std::invalid_argument("[random_uniform_phases] Min value (" + std::to_string(min_val) + ") cannot be greater than max value (" + std::to_string(max_val) + ").");
        }

        // Original logic
        std::mt19937 rng(seed);
        std::uniform_real_distribution<double> dist(min_val, max_val);
        dVec  phases(N);
        for (auto& x : phases)
            x = dist(rng);
        return phases;
    }
    ```

   **Example Usage:**

   ```C++
   size_t N = 100; // Number of oscillators
   double min_val = 0.0;
   double max_val = 2 * PI;
   dVec initial_phases = random_uniform(N, min_val, max_val);
   ```

    This will create a vector `initial_phases` with 100 phases uniformly distributed between 0 and 2π.

2. **Random Normal Initialization**

   **Function Name:** `random_normal`

   **Description:** This function initializes the phases of oscillators randomly with a normal (Gaussian) distribution with specified mean and standard deviation.

   **Parameters:**
   - `size_t N`: The number of oscillators to initialize.
   - `double mean`: The mean value for the normal distribution.
   - `double stddev`: The standard deviation for the normal distribution.
   - `unsigned int seed` (optional): Seed for the random number generator to ensure reproducibility.

   **Returns:** A vector of size `N` containing the initialized phases.

    **Implementation:**

    ```C++
    // Normal distribution with given mean and stddev
    inline dVec  random_normal(
        size_t   N,
        double   mean,
        double   stddev,
        unsigned seed
    )
    {
        // Error Handling
        if (N == 0) 
        {
            throw std::invalid_argument("[random_normal_phases] Number of phases N cannot be zero.");
        }
        if (stddev < 0) 
        {
            throw std::invalid_argument("[random_normal_phases] Standard deviation cannot be negative.");
        }

        // Original logic
        std::mt19937 rng(seed);
        std::normal_distribution<double> dist(mean, stddev);
        dVec  phases(N);
        for (auto& x : phases)
            x = dist(rng);
        return phases;
    }
    ```

   **Example Usage:**

   ```C++
   size_t N = 100; // Number of oscillators
   double mean = PI; // Mean phase
   double stddev = 0.5; // Standard deviation
   dVec initial_phases = random_normal(N, mean, stddev);
   ```

    This will create a vector `initial_phases` with 100 phases normally distributed around π with a standard deviation of 0.5.

3. **Random Cauchy Initialization**

   **Function Name:** `random_cauchy`

   **Description:** This function initializes the phases of oscillators randomly with a Cauchy distribution with specified location and scale parameters.

   **Parameters:**
   - `size_t N`: The number of oscillators to initialize.
   - `double location`: The location parameter for the Cauchy distribution.
   - `double scale`: The scale parameter for the Cauchy distribution.
   - `unsigned int seed` (optional): Seed for the random number generator to ensure reproducibility.

   **Returns:** A vector of size `N` containing the initialized phases.

    **Implementation:**

    ```C++
    // Cauchy distribution with given location and scale
    inline dVec  random_cauchy(
        size_t   N,
        double   location,
        double   scale,
        unsigned seed
    )
    {
        // Error Handling
        if (N == 0) 
        {
            throw std::invalid_argument("[random_cauchy_phases] Number of phases N cannot be zero.");
        }
        if (scale <= 0) 
        {
            throw std::invalid_argument("[random_cauchy_phases] Scale parameter must be positive.");
        }

        // Original logic
        std::mt19937 rng(seed);
        std::cauchy_distribution<double> dist(location, scale);
        dVec  phases(N);
        for (auto& x : phases)
            x = dist(rng);
        return phases;
    }
    ```

   **Example Usage:**

   ```C++
   size_t N = 100; // Number of oscillators
   double location = PI; // Location parameter
   double scale = 0.1; // Scale parameter
   dVec intrinsic_frequencies = random_cauchy(N, location, scale);
   ```

    This will create a vector `intrinsic_frequencies` with 100 frequencies distributed according to a Cauchy distribution centered at π with a scale of 0.1.

4. **Random Exponential Initialization**

   **Function Name:** `random_exponential`

   **Description:** This function initializes the parameters of oscillators randomly with an exponential distribution with a specified rate parameter.

   **Parameters:**
   - `size_t N`: The number of oscillators to initialize.
   - `double rate`: The rate parameter (λ) for the exponential distribution.
   - `unsigned int seed` (optional): Seed for the random number generator to ensure reproducibility.

   **Returns:** A vector of size `N` containing the initialized parameters.

    **Implementation:**

    ```C++
    // Exponential distribution with given rate
    inline dVec  random_exponential(
        size_t   N,
        double   rate,
        unsigned seed
    )
    {
        // Error Handling
        if (N == 0) 
        {
            throw std::invalid_argument("[random_exponential_params] Number of parameters N cannot be zero.");
        }
        if (rate <= 0) 
        {
            throw std::invalid_argument("[random_exponential_params] Rate parameter must be positive.");
        }

        // Original logic
        std::mt19937 rng(seed);
        std::exponential_distribution<double> dist(rate);
        dVec  params(N);
        for (auto& x : params)
            x = dist(rng);
        return params;
    }
    ```

   **Example Usage:**

   ```C++
   size_t N = 100; // Number of oscillators
   double rate = 1.0; // Rate parameter
   dVec coupling_strengths = random_exponential(N, rate);
   ```

    This will create a vector `coupling_strengths` with 100 coupling strengths distributed according to an exponential distribution with a rate of 1.0.

5. **Random Circle Initialization**

   **Function Name:** `random_circle`

   **Description:** This function initializes points randomly on the unit circle.

   **Parameters:**
   - `size_t N`: The number of points to initialize.
   - `unsigned int seed` (optional): Seed for the random number generator to ensure reproducibility.

   **Returns:** A vector of size `N` containing the initialized points on the unit circle.

    **Implementation:**

    ```C++
    // Random points on the unit circle
    inline dVec  random_circle(
        size_t   N,
        unsigned seed
    )
    {
        // Error Handling
        if (N == 0) 
        {
            throw std::invalid_argument("[random_circle_points] Number of points N cannot be zero.");
        }

        // Original logic
        std::mt19937 rng(seed);
        std::uniform_real_distribution<double> dist(0.0, 2 * PI);
        dVec  points(N);
        for (auto& x : points)
            x = dist(rng);
        return points;
    }
    ```

   **Example Usage:**

   ```C++
   size_t N = 100; // Number of points
   dVec circle_points = random_circle(N);
   ```

    This will create a vector `circle_points` with 100 points uniformly distributed on the unit circle.

6. **Splay Initialization**

    **Function Name:** `splay`

    **Description:** This function initializes the phases of oscillators in a splay (equally spaced) configuration around the unit circle.

    **Parameters:**
    - `size_t N`: The number of oscillators to initialize.

    **Returns:** A vector of size `N` containing the initialized phases.

    **Implementation:**

    ```C++
    // Splay phases: equidistant around the circle [0, 2*pi)
    inline dVec  splay(
        size_t N
    )
    {
        // Error Handling
        if (N == 0) 
        {
            throw std::invalid_argument("[splay_phases] Number of phases N cannot be zero.");
        }

        dVec  phases(N);
        if (N == 1) 
        {
            phases[0] = 0.0;
            return phases;
        }

        double delta = 2.0 * PI / static_cast<double>(N);
        for (size_t i = 0; i < N; ++i)
            phases[i] = i * delta;
        return phases;
    }
    ```

    **Example Usage:**

    ```C++
    size_t N = 100; // Number of oscillators
    dVec phases = splay(N);
    ```

    This will create a vector `phases` with 100 phases equally spaced around the unit circle.

7. **Splay Disturbed Initialization (random offset)**

    **Function Name:** `splay_disturbed`

    **Description:** This function initializes the phases of oscillators in a splay configuration with a small random disturbance added to each phase.

    **Parameters:**
    - `size_t N`: The number of oscillators to initialize.
    - `double amplitude`: The maximum amplitude of the random disturbance added to each phase.
    - `unsigned int seed` (optional): Seed for the random number generator to ensure reproducibility.

    **Returns:** A vector of size `N` containing the initialized splay disturbed phases.

    **Implementation:**

    ```C++
    // Splay phases with random perturbation in [-amplitude, amplitude]
    inline dVec  splay_perturbed(
        size_t   N,
        double   amplitude,
        unsigned seed
    )
    {
        // Error Handling
        if (amplitude < 0.0) 
        {
            throw std::invalid_argument("[splay_phases_perturbed] Amplitude (" + std::to_string(amplitude) + ") cannot be negative.");
        }

        auto phases = splay(N);
        if (N == 0) return phases;

        std::mt19937 rng(seed);
        std::uniform_real_distribution<double> dist(-amplitude, amplitude);
        for (auto& x : phases)
            x += dist(rng);
        return phases;
    }
    ```

    **Example Usage:**

    ```C++
    size_t N = 100; // Number of oscillators
    double amplitude = 0.1; // Maximum amplitude of the random disturbance
    dVec phases = splay_perturbed(N, amplitude);
    ```

    This will create a vector `phases` with 100 phases equally spaced around the unit circle, each perturbed by a random value in the range [-0.1, 0.1].

8. **Module by Condition**

    **Function Name:** 'module_by_condition'

    **Description:** This function initializes a vector based on a specified condition. If the condition is true, the vector is filled with one set of values; otherwise, it is filled with another set of values. This is particularly useful for initializing parameters of a peicewise-defined model (i.e. modular system).

    **Parameters:**
    - `size_t module_size`: The size of the vector to initialize.
    - `std::string dist_type`: A string representing the condition to evaluate.
    - `double a`: First value to use if the condition is true. (Technically a placeholder for values such as mean, location, etc. depending on the distribution type).
    - `double b`: Second value to use if the condition is true. (Technically a placeholder for values such as stddev, scale, etc. depending on the distribution type).
    - `usigned int seed` (optional): Seed for the random number generator to ensure reproducibility.

    **Returns:** A vector of size `module_size` containing the initialized values based on the condition (distribution type and corresponding variables dictating distribution shape).

    **Implementation:**

    ```C++
    // Generate phases/frequencies for one module based on a given condition (uniform, normal, etc)
    inline dVec  module_by_condition(
        size_t                module_size,
        const std::string&    dist_type,
        double                a,
        double                b,
        unsigned              seed
    )
    {
        // Error Handling
        if (module_size == 0) 
        {
            throw std::invalid_argument("[module_by_condition] module_size cannot be zero for distribution type '" + dist_type + "'.");
        }

        if (dist_type == "uniform")
        {
            return random_uniform(module_size, a, b, seed);
        }
        else if (dist_type == "normal")
        {
            return random_normal(module_size, a, b, seed);
        }
        else if (dist_type == "cauchy")
        {
            return random_cauchy(module_size, a, b, seed);
        }
        else if (dist_type == "exponential")
        {
            return random_exponential(module_size, a, seed);
        }
        else if (dist_type == "circle")
        {
            return random_circle(module_size, seed);
        }
        else if (dist_type == "splay")
        {
            return splay(module_size);
        }
        else if (dist_type == "splay_perturbed")
        {
            return splay_perturbed(module_size,a,seed);
        }
        else
        {
            throw std::invalid_argument("[module_by_condition] Unknown distribution type: '" + dist_type + "'.");
        }
    }
    ```

    **Example Usage:**

    ```C++
    size_t module_size = 50; // Size of the module
    std::string dist_type = "normal"; // Distribution type
    double mean = PI; // Mean for normal distribution
    double stddev = 0.5; // Standard deviation for normal distribution
    auto phases = module_by_condition(module_size, dist_type, mean, stddev);
    ```

    This will create a vector `phases` with 50 phases initialized according to a normal distribution with mean π and standard deviation 0.5.

9. **Identical Modules (repeated values of the `module_by_condition` function)**

    **Function Name:** `identical_modules`

    **Description:** This function initializes a larger vector by repeating the initialization of smaller modules based on a specified condition. It uses the `module_by_condition` function to generate each module and concatenates them to form the final vector.

    **Parameters:**
    - `size_t num_modules`: The number of modules to create.
    - `size_t module_size`: The size of each module.
    - `std::string dist_type`: A string representing the condition to evaluate for each module.
    - `double a`: First value to use if the condition is true for each module.
    - `double b`: Second value to use if the condition is true for each module.
    - `unsigned int seed` (optional): Seed for the random number generator to ensure reproducibility.

    **Returns:** A vector of size `num_modules * module_size` containing the initialized values based on the condition for each module.

    **Implementation:**

    ```C++
    // Generate phases/frequencies for one module and copy to all modules (identical modules)
    inline dVec  identical_modules(
        size_t               N_per_module,
        size_t               num_modules,
        const std::string&   dist_type,
        double               a,
        double               b,
        unsigned             seed
    )
    {
        // Error Handling
        if (N_per_module == 0) 
        {
            throw std::invalid_argument("[identical_modules] N_per_module (nodes per module) cannot be zero.");
        }
        if (num_modules == 0) 
        {
            throw std::invalid_argument("[identical_modules] num_modules cannot be zero.");
        }

        dVec  base = module_by_condition(N_per_module, dist_type, a, b, seed);
        dVec  result(N_per_module * num_modules);
        for (size_t m = 0; m < num_modules; ++m)
        {
            for (size_t i = 0; i < N_per_module; ++i)
            {
                result[m * N_per_module + i] = base[i];
            }
        }
        return result;
    }
    ```

    **Example Usage:**

    ```C++
    size_t num_modules = 3; // Number of modules
    size_t module_size = 50; // Size of each module
    std::string dist_type = "uniform"; // Distribution type
    double min_val = 0.0; // Min value for uniform distribution
    double max_val = 2 * PI
