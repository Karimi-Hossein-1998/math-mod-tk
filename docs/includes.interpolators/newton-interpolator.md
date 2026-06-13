# [Newton Interpolator](../../includes/interpolators/newton-interpolator.hpp)

The Newton interpolator is a form of constructing the interpolating function by using the *Newton's divided difference method*. The resulting interpolant works just like the Lagrange interpolation method, the difference lies within their computational profile (interested users can find related information online or most textbooks regarding *Numerical Analysis*.).

## Table of Contents

1. [Newton Interpolator](#newton-interpolator)
    1. [Table of Contents](#table-of-contents)
    2. [Mathematical Formulation](#mathematical-formulation)
    3. [Functions Implemented](#functions-implemented)

## Mathematical Formulation

Assume we have the set of points $ (x_i, y_i) \text(for) i \in \{0,1,2,\ldots,n\} $ which fulfil the following condition:

$$ \displaystyle y_i = f(x_i) = P(x_i) $$

Then we assume that the estimate function (i.e. the interpolant) $P(x)$ has the form:

$$ \displaystyle P(x) = a_0 + \sum_{i=1}^{n} a_i \times \prod_{j=0}^{i-1} (x-x_j). $$

By pluging in $x=x_0$ we get that $a_0 = y_0$, we can keep repeating this process until we get the values of all our coefficients ($a_i$s).

$$\Large \displaystyle \begin{cases} a_0 &= y_0 \\[0.3cm] a_1 &= \frac{y_1 - y_0}{x_1-x_0} \\[0.3cm] a_2 &= \frac{\frac{y_2-y_0}{x_2-x_0}-\frac{y_1-y_0}{x_1-x_0}}{x_2-x_1} \\[0.3cm] \vdots &\vdots\quad \vdots \end{cases} $$

Hence the name *divided difference*.

## Functions Implemented

1. **Newton Interpolator (single-valued function)**

    **Function Name:** `newton_interpolator`

    **Description:** This function works just like the [Lagrange Interpolator (single-valued function)](lagrange-interpolator.md/#functions-implemented), the difference is that to create the interpolating function we replace the *Lagrange* method with the *[Newton's divided difference](#mathematical-formulation)* method.

    **Parameters:**
    - `const dVec& x_points`: The **$x$** points of the *known* set of points.
    - `const dVec& y_points`: The corresponding **$y$** points of the *known* set of points.

    **Returns:** Returns a lambda function of the form `std::function<double(double)>` that takes `double x` and returns the estimated/interpolated value of the function as a `double`.

    **Implementation:**

    ```C++
    // Newton interpolator: returns a function that interpolates at any x
    inline func newton_interpolator(const dVec& x_points, const dVec& y_points)
    {
        // Compute divided differences
        size_t n = x_points.size();
        dVec coef = y_points;
        for (size_t j = 1; j < n; ++j)
        {
            for (size_t i = n - 1; i >= j; --i)
            {
                coef[i] = (coef[i] - coef[i - 1]) / (x_points[i] - x_points[i - j]);
            }
        }
        return [x_points, coef](double x) -> double {
            double result = coef.back();
            for (int i = static_cast<int>(coef.size()) - 2; i >= 0; --i)
            {
                result = result * (x - x_points[i]) + coef[i];
            }
            return result;
        };
    }
    ```

    **Example Usage:**

    ```C++
    #include "path/to/includes/interpolators/newton-interpolator.hpp"
    
    // The rest of the code
    // ...
    // ...
    // ...
    // The usage

    dVec x_points = {0.0, 1.0, 2.0, 3.0};
    dVec y_points = {-1.0, 0.0, 3.0, 8.0};
    funt interpolator = newton_interpolator(x_points, y_points);
    x = 1.25;
    auto y_estimate = interpolator(x) // This will store the estimated value of the function that is described by the datapoints (x_i, y_i) for x_i in x_points and y_i in y_points, y_estimate = P(x) is a double value.
    ```

2. **Newton Interpolator (vector-valued function)**

    **Function Name:** `newton_interpolator` (overloaded)

    **Description:** This function works just like the [Lagrange Interpolator (vector-valued function)](lagrange-interpolator.md/#functions-implemented), the difference is that to create the interpolating function we replace the *Lagrange* method with the *[Newton's divided difference](#mathematical-formulation)* method.

    **Parameters:**
    - `const dVec& x_points`: The **$x$** points of the *known* set of points.
    - `const dVec& y_points`: The corresponding **$y$** points of the *known* set of points.

    **Returns:** Returns a lambda function of the form `std::function<double(double)>` that takes `double x` and returns the estimated/interpolated value of the function as a `double`.

    **Implementation:**

    ```C++
    // Newton interpolator: returns a function that interpolates at any x
    inline Func newton_interpolator(const dVec& x_points, const dMatrix& y_points)
    {
        size_t n = x_points.size();
        size_t N = y_points[0].size();
        // Compute divided differences for each component
        dMatrix coef = y_points;
        for (size_t j = 1; j < n; ++j)
        {
            for (size_t i = n - 1; i >= j; --i)
            {
                for (size_t k = 0; k < N; ++k)
                {
                    coef[i][k] = (coef[i][k] - coef[i - 1][k]) / (x_points[i] - x_points[i - j]);
                }
            }
        }
        return [x_points, coef, n, N](double x) -> dVec {
            dVec result(N, 0.0);
            for (size_t k = 0; k < N; ++k)
            {
                double value = coef[n - 1][k];
                for (int i = static_cast<int>(n) - 2; i >= 0; --i)
                {
                    value = value * (x - x_points[i]) + coef[i][k];
                }
                result[k] = value;
            }
            return result;
        };
    } 
    ```

    **Example Usage:**

    ```C++
    #include "path/to/includes/interpolators/newton-interpolator.hpp"
    
    // The rest of the code
    // ...
    // ...
    // ...
    // The usage

    dVec x_points = {0.0, 1.0, 2.0, 3.0};
    dMatrix y_points = {{-1.0, 0.0, 3.0, 8.0}, {2.0, 0.0, -5.0, -1.0},{-2.0, 1.5, -0.5, 3.25}};
    Func interpolator = newton_interpolator(x_points, y_points);
    x = 1.25;
    auto y_estimate = interpolator(x) // This will store the estimated value of the function that is described by the datapoints (x_i, y_i) for x_i in x_points and y_i in y_points, y_estimate = P(x) is a double value.
    ```
