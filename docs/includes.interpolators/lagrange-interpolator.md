# [Lagrange Interpolator](../../includes/interpolators/lagrange-interpolator.hpp)

The lagrange interpolator is a polynomial interpolator that uses the Lagrange form to estimate values between known data points. It constructs a polynomial that passes through all given points and can be used for interpolation.

## Table of Contents

1. [Lagrange Interpolator](#lagrange-interpolator)
    1. [Table of Contents](#table-of-contents)
    2. [Mathematical Background](#mathematical-background)
        1. [Standard Form](#standard-form)
        2. [Barycentric Form](#barycentric-form)
    3. [Functions Implemented](#functions-implemented)

## Mathematical Background

There is two main methods of expressing the Lagrange interpolating polynomial. The first one is the *standard* form, and the second one is the *barycentric* form.

### Standard Form

Assume we have a set of *known* data points of values over the domain and ranges a function $f(x)$, as $\{(x_0, y_0), (x_1, y_1), \ldots, (x_n, y_n)\}$, where $y_i = f(x_i)$ for $i = 0, 1, \ldots, n$. The Lagrange interpolating polynomial $P(x)$ is given by:

$$ \displaystyle P(x) = \sum_{i=0}^{n} y_i L_i(x) $$

where the $L_i(x)$ follows this orthogo-normalization rule:

$$ \displaystyle L_i(x_j) = \delta_{ij} = \begin{cases} 1 \; & \text{if } i = j \\ 0 \; & \text{if } i \neq j \end{cases} $$

hence $L_i(x)$ are the Lagrange basis polynomials defined as:

$$ \displaystyle L_i(x) = \prod_{\substack{0 \leq j \leq n \\ j \neq i}} \frac{x - x_j}{x_i - x_j} $$

### Barycentric Form

The barycentric form of the Lagrange interpolating polynomial is designed for numerical stability and efficiency. It is given by:

first we define the weights $w_j$ as:

$$ \displaystyle w_j = \prod_{\substack{0 \leq k \leq n \\ k \neq j}} \frac{1}{x_j - x_k} $$

then the interpolant would be:

$$\large \displaystyle P(x) = \frac{\sum_{j=0}^{n} \frac{w_j y_j}{x - x_j}}{\sum_{j=0}^{n} \frac{w_j}{x - x_j}} $$

## Functions Implemented

1. **Lagrange Interpolator (single-valued function)**

    **Function Name:** `lagrange_interpolator`

    **Description:** This function takes two sets named `x_points` and `y_points`, and returns a `func` (`std::function<double(double)>`), which is a one to one map.

    **Parameters:**
    - `const dVec& x_points`: The **$x$** points of the *known* set of points.
    - `const dVec& y_points`: The corresponding **$y$** points of the *known* set of points.

    **Returns:** Returns a lambda function of the form `std::function<double(double)>` that takes `double x` and returns the estimated/interpolated value of the function as a `double`.

    **Implementation:**

    ```C++
    // Lagrange interpolator: returns a function that interpolates at any x
    inline func lagrange_interpolator(const dVec& x_points, const dVec& y_points)
    {
        return [x_points, y_points](double x) -> double {
            double result = 0.0;
            size_t n = x_points.size();
            for (size_t i = 0; i < n; ++i)
            {
                double term = y_points[i];
                for (size_t j = 0; j < n; ++j)
                {
                    if (i != j)
                        term *= (x - x_points[j]) / (x_points[i] - x_points[j]);
                }
                result += term;
            }
            return result;
        };
    }
    ```

    **Example Usage:**

    ```C++
    #include "path/to/includes/interpolators/lagrange-interpolator.hpp"
    
    // The rest of the code
    // ...
    // ...
    // ...
    // The usage

    dVec x_points = {0.0, 1.0, 2.0, 3.0};
    dVec y_points = {-1.0, 0.0, 3.0, 8.0};
    func interpolator = lagrange_interpolator(x_points, y_points);
    x = 1.25;
    auto y_estimate = interpolator(x) // This will store the estimated value of the function that is described by the datapoints (x_i, y_i) for x_i in x_points and y_i in y_points, y_estimate = P(x) is a double value.
    ```

2. **Lagrange Interpolator (vector-valued function)**

    **Function Name:** `lagrange_interpolator` (overloaded)

    **Description:** This function takes two sets named `x_points` and `y_points`, and returns a `Func` (`std::function<dVec(double)>`), which is a one to one map.

    $$ \displaystyle \overrightarrow{P}(x) \approx \overrightarrow{y} = \overrightarrow{f}(x). $$

    **Parameters:**
    - `const dVec& x_points`: The **$x$** points of the *known* set of points.
    - `const dMatrix& y_points`: The corresponding **$y$** points of the *known* set of points.

    **Returns:** Returns a lambda function of the form `std::function<dVec(double)>` that takes `double x` and returns the estimated/interpolated values of the functions as a `dVec`.

    **Implementation:**

    ```C++
    // Vector-valued Lagrange interpolator: returns a function that interpolates at any x
    inline Func lagrange_interpolator(const dVec& x_points, const dMatrix& y_points)
    {
        return [x_points, y_points](double x) -> dVec {
            size_t n = x_points.size();
            size_t N = y_points[0].size();
            dVec result(N, 0.0);
            for (size_t k = 0; k < N; ++k)
            {
                // Interpolate the k-th component
                double value = 0.0;
                for (size_t i = 0; i < n; ++i)
                {
                    double term = y_points[i][k];
                    for (size_t j = 0; j < n; ++j)
                    {
                        if (i != j)
                            term *= (x - x_points[j]) / (x_points[i] - x_points[j]);
                    }
                    value += term;
                }
                result[k] = value;
            }
            return result;
        };
    }
    ```

    **Example Usage:**

    ```C++
    dVec x_points = {0.0, 1.0, 2.0, 3.0};
    dMatrix y_points = {{-1.0, 0.0, 3.0, 8.0}, {2.0, 0.0, -5.0, -1.0},{-2.0, 1.5, -0.5, 3.25}};
    Func interpolator = lagrange_interpolator(x_points, y_points);
    x = 1.25;
    auto y_estimate = interpolator(x) // This will store the estimated value of the function that is described by the datapoints (x_i, {y_i}) for x_i in x_points and y_i in y_points, y_estimate = P(x) is a vector of double values.
    ```

3. **Barycentric Form (single-valued function)**

    **Function Name:** `barycentric_lagrange_interpolator`

    ...The same as the **Lagrange Interpolator (single-valued function)**, but uses different form of calculations.

4. **Barycentric Form (vector-valued function)**

    **Function Name:** `barycentric_lagrange_interpolator` (overloaded)

    ...The same as the **Lagrange Interpolator (vector-valued function)**, but uses different form of calculations.
