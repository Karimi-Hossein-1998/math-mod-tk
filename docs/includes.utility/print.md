# Print

The header file [`print.hpp`](../../includes/utility/print.hpp) provides general purpose output functionality (i.e. writing values of a vector or matrix to the terminal, printing a progress bar, etc.)

## Table of Contents

1. [Print](#print)
    1. [Table of Contents](#table-of-contents)
    2. [Functions](#functions)

## Functions

The functions are:

1. **Print Data:** For printing data the following functions are available.
    1. **Vector Printing:**

        **Function Name:** `print_vector`

        **Input:** The function takes a single input, which is a `std::vector` of any type `t` (denoted as `Vec<t>`).

        **Output:** The function does not return any value; it outputs the contents of the vector to the standard output (console) in a formatted manner.

        **Purpose:** The purpose of the function is to write a `std::vector<t>` in a readable format.

        **Implementation:**

        ```C++
            template <typename t>
            void print_vector(const Vec<t>& vector)
            {
                size_t size = vector.empty() ? 0 : vector.size();
                std::cout << "Vector (" << size << "):" << std::endl;
                std::cout << "[ ";
                for (size_t v = 0; v < vector.size(); ++v)
                {
                    std::cout << vector[v];
                    if ( v + 1 < vector.size()) std::cout << ", ";
                }
                std::cout << " ]";
                std::cout << std::endl;
            }
        ```

        **Example:**

        ```C++
            dVec vec1 = {0.0, -1.25, 2.33};
            print_vector(vec1);
        ```

        This will output:

        ```bash
            Vector (3):
            [ 0.0, -1.25, 2.33 ]
        ```

    2. **dMatrix Printing:**

        **Function Name:** `print_matrix`

        **Input:** The function takes a single input, which is a `std::vector` of `std::vector`s (denoted as `Vec<Vec<t>>`), representing a matrix of any type `t`.

        **Output:** The function does not return any value; it outputs the contents of the matrix to the standard output (console) in a formatted manner.

        **Purpose:** The purpose of the function is to write a `std::vector<std::vector<t>>` in a readable format.

        **Implementation:**

        ```C++
            template <typename t>
            void print_matrix(const Vec<Vec<t>>& matrix)
            {
                std::cout << "dMatrix (" << matrix.size() << "x" << (matrix.empty() ? 0 : matrix[0].size()) << "): " << std::endl;
                std::cout << "[ ";
                for (size_t r = 0; r < matrix.size(); ++r)
                {
                    const auto& row = matrix[r];
                    std::cout << "[ ";
                    for (size_t i = 0; i < row.size(); ++i)
                    {
                        std::cout << row[i];
                        if (i + 1 < row.size()) std::cout << ", ";
                    }
                    std::cout << " ]";
                    if (r + 1 < matrix.size()) std::cout << ",";
                    std::cout << std::endl;
                }
                std::cout << " ]" << std::endl;
            }
        ```

        **Example:**

        ```C++
            dMatrix matrix = {{0.0,-2.05,1.25},
                             {2.05,0.0,-2.33},
                             {-1.25,2.33,0.0}};
            print_matrix(matrix);
        ```

        This will output:

        ```bash
            dMatrix (3x3):
            [ [ 0.0, -2.05, 1.25 ],
              [ 2.05, 0.0, -2.33 ],
              [ -1.25, 2.33, 0.0 ] ]
        ```

2. **Progress Bar:** For printing a progress bar the following function is available.

    1. **Only progress bar**

        **Function Name:** `print_progress_bar`

        **Input:** The function takes three inputs:
        - `size_t current`: This is the current progress value, which indicates how much of the task has been completed.
        - `size_t total`: This is the total value that represents the completion of the task. When `current` equals `total`, it indicates that the task is fully completed.
        - `size_t bar_width`: This is the width of the progress bar in characters. It determines how wide the visual representation of the progress bar will be in the console.

        **Output:** The function does not return any value; it outputs a visual representation of the progress bar to the standard output (console).

        **Purpose:** The purpose of the function is to provide a visual indication of progress for a task, allowing users to see how much of the task has been completed and how much remains.

        **Implementation:**

        ```C++
            void print_progress_bar(size_t current, size_t total, size_t bar_width = 50)
            {
                double progress = static_cast<double>(current) / total;
                size_t pos = static_cast<size_t>(bar_width * progress);
                std::cout << "\rProgress: [";
                for (size_t i = 0; i < bar_width; ++i)
                {
                    if (i < pos) std::cout << "=";
                    else if (i == pos) std::cout << ">";
                    else std::cout << ".";
                }
                std::cout << "] ";
                auto percentage = static_cast<size_t>(progress * 100.0);
                if ( percentage != 100)
                    std::cout << std::setw(3) << percentage << "%";
                else
                    std::cout << "Done!";
                std::cout.flush();
            }
        ```

    2. **Progress bar with eta (estimated time of )**

        **Function Name:** `print_progress_bar_eta`

        **Input:**

        **Purpose:** This function serves the same purpose as `print_progress_bar`, but it also includes an additional feature to display the estimated time of arrival (ETA) for the completion of the task. This allows users to not only see the progress but also have an estimate of how much time is remaining until the task is completed.

        **Input:** The function takes four inputs:
        - `size_t current`: This is the current progress value, which indicates how much of the task has been completed.
        - `size_t total`: This is the total value that represents the completion of the task. When `current` equals `total`, it indicates that the task is fully completed.
        - `T eta`: This is the estimated time of arrival (ETA) for the completion.
        - `size_t bar_width`: This is the width of the progress bar in characters. It determines how wide the visual representation of the progress bar will be in the console.

        **Output:** The function does not return any value; it outputs a visual representation of the progress bar along with the ETA to the standard output (console).

        **Implementation:**

        ```C++
            template <typename T>
            void print_progress_bar_eta(size_t current, size_t total, T eta, size_t bar_width = 50)
            {
                double progress = static_cast<double>(current) / total;
                size_t pos = static_cast<size_t>(bar_width * progress);
                std::cout << "\rProgress: [";
                for (size_t i = 0; i < bar_width; ++i)
                {
                    if (i < pos) std::cout << "=";
                    else if (i == pos) std::cout << ">";
                    else std::cout << ".";
                }
                std::cout << "] ";
                auto percentage = static_cast<size_t>(progress * 100.0);
                if ( percentage != 100)
                    std::cout << std::setw(3) << percentage << "%";
                else
                    std::cout << "Done!";
                std::cout << " : eta = " << eta;
                std::cout.flush();
            }
        ```
