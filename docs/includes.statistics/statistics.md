# Statistics

## Table of Contents

1. [Statistics]{#statistics}
    1. [Table of Contents]{#table-of-contents}
    2. [Introduction]{#introduction}
    3. [Statistics Header]{#statistics-header}

## Introduction

The *statistics* section has two files [statistics.hpp]{../../includes/statistics/statistics.hpp} and [statistics-extended.hpp]{../../includes/statistics/statistics-extended.hpp}. Main statistics functions are defined in the `statistics.hpp` header file.

## Statistics Header

It includes functions for *mean value*, *variance*, *standard deviation*, and *coefficient of variability* for **vectors** and **matrices**.

### Functions

1. **Mean Value:** Calculates mean value
    - **Name:** `mean_value`
    - **Arguments:** `dVec`
    - **Output:** `double`
    - **Usage:**

        ```C++
        #include "path/to/statistics.hpp"

        dVec V = {1.0, 1.0, 2.0, 3.0, 3.0, 3.0, 4.0, 4.0, 5.0, 6.0, 6.0, 10.0};
        double mean = 0.0;
        mean = mean_value(V);
        std::cout << "Mean value is: " << mean << std::endl;
        ```

        The output would be `Mean value is: 4`.

2. **Mean Value 0:** Calculates mean value for all the rows of a given matrix.
    - **Name:** `mean_value0`
    - **Arguments:** `dMatrix`
    - **Output:** `dVec`
    - **Usage:**

        ```C++
        dMatrix M = {{1.0, 3.0, 5.0, 7.0},
                     {2.0, 4.0, 6.0, 8.0},
                     {3.0, 5.0, 7.0, 9.0}};
        dVec mean_row = mean_value0(M);
        std::cout << "[ " << std::endl;
        for ( auto mean : mean_row )
        {
            std::cout << mean << std::endl;
        }
        std::cout << "\n]" << std::endl;
        ```

        The output would be:

        ```bash
        [
        4
        5
        6
        ]
        ```

3. **Mean Value 1:** Calculates mean value for all the columns of a given matrix.
    - **Name:** `mean_value1`
    - **Arguments:** `dMatrix`
    - **Output:** `dVec`
    - **Usage:**

        ```C++
        dMatrix M = {{1.0, 3.0, 5.0, 7.0},
                     {2.0, 4.0, 6.0, 8.0},
                     {3.0, 5.0, 7.0, 9.0}};
        dVec mean_col = mean_value1(M);
        std::cout << "[ " << " ";
        for ( auto mean : mean_col )
        {
            std::cout << mean << " ";
        }
        std::cout << " ]" << std::endl;
        ```

        The output would be:

        ```bash
        [ 2 4 6 8 ]
        ```

4. **Mean value (matrix version):** Returns mean value of a given matrix and decides rows or columns based on a `mode` parameter.

    - **Name:** `mean_value`
    - **Arguments:** `dMatrix`, `size_t`
    - **Output:** `dVec`
    - **Usage:**

        ```C++
        dMatrix M = {{1.0, 3.0, 5.0, 7.0},
                     {2.0, 4.0, 6.0, 8.0},
                     {3.0, 5.0, 7.0, 9.0}};
        dVec mean_row = mean_value(M, 0);
        dVec mean_col = mean_value(M, 1);
        std::cout << "[ " << std::endl;
        for ( auto mean : mean_row )
        {
            std::cout << mean << std::endl;
        }
        std::cout << "\n]" << std::endl;
        std::cout << "[ " << " ";
        for ( auto mean : mean_col )
        {
            std::cout << mean << " ";
        }
        std::cout << " ]" << std::endl;
        ```

5. **Variance:** Calculates variance (it also has for different function calls like mean value).
    - **Names:** `variance`, `variance0`, `variance1`, `variance`

6. **Standard deviance:** Calculates standard deviation (four function calls).
    - **Names:** `stddev`, `stddev0`, `stddev1`, `stddev`

7. **Coefficient of variability:** Calculates coefficient of variability (four function calls).
    - **Names:** `coefficient_of_variability`, `coefficient_of_variability0`, `coefficient_of_variability1`, `coefficient_of_variability`

8. **Statistics:** Calculates *mean value*, *standard deviation*, and *cv*.
    - **Name:** `statistics`
    - **Arguments:** `dVec`
    - **Output:** `dVec {mean, stddev, cv}`

9. **Statistics 0/1:** The same as statistics applied to rows/columns of a matrix.
    - **Name:** `statistics0`
    - **Arguments:** `dMatrix`
    - **Output:** `dMatrix`

    The output's shape:

    ```C++
    {{mean 0, stddev 0, cv 0},
     {mean 1, stddev 1, cv 1},
      .         .         .  
      .         .         .
      .         .         .}
    ```

    Also there is `statistics` for matrices with `mode` argument to choose between `0`, and `1`.

## Statistics-Extended

Imagine you have a time series, if you need to find out whether it has a stationary limit after a certain point you might proceed with calculating averages over last timepoints from a given point for multiple final segments with differing sizes. In the header `statistics-extended.hpp` I have tried to aleviate that need.

**List of Functions:**

1. **Statistics of the Last $n$ Percent:** Calculates the statistics of the last $n$ percent of a given vector.
    - **Name:** `statistics_last_n_percent`
    - **Arguments:** `dVec`, `dVec`
    - **Output:** `dMatrix = Vec {dVec {mean, stddev, cv} }`
    - **Usage:**

    ```C++
    dVec V = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0};
    dVec percentages = {0.05, 0.1, 0.15, 0.2};
    auto stats = statistics_last_n_percent(V, percentages);
    ```

    The `stats` would be roughly something like this

    ```text
    { { 20.0, 0.0, 0.0},
      { 19.5, 0.3547, ...},
      { 19.0, 0.4714,...},
      { 18.5, 0.5590,...}
    }
    ```

2. **Statistics of the Last $n$ Percent for Matrices:** The same as above but for matrices and with a `mode` argument to choose between rows and columns.
    - **Name:** `statistics_last_n_percent`
    - **Arguments:** `dMatrix`, `dVec`, `size_t`
    - **Output:** `Vec{dMatrix} = Vec{ Vec {dVec {mean, stddev, cv} } }`

    It would generate a vector of matrices with the size of the free axis (the axis that is not chosen by the `mode` argument) and each matrix would have the same number of rows as the `percentages` vector, and with three columns for mean, stddev, and cv.
