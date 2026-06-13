# Write

The header file [`write.hpp`](../../includes/utility/write.hpp) provides general purpose file output functionality (i.e. writing values of a vector or matrix to a file with flexible formatting options, binary export, etc.)

## Table of Contents

1. [Write](#write)
    1. [Table of Contents](#table-of-contents)
    2. [Functions](#functions)
    3. [Usage Notes](#usage-notes)

## Functions

The functions are:

1. **Write Data to File:** For writing data to files the following functions are available.

    1. **dMatrix Writing:**

        **Function Name:** `write_matrix_to_file`

        **Input:** The function takes multiple inputs:
        - `const Vec<Vec<t>>& matrix`: The matrix to write (2D vector of any type `t`).
        - `const std::string& directory`: Output directory path.
        - `const std::string& filename`: Output filename without extension.
        - `const std::string& filetype`: File extension (default: `"txt"`).
        - `const std::string& separator`: Separator between values (default: `" "`).
        - `const std::string& comment`: Optional comment string (added after header).
        - `const std::string& precision`: Number of digits after decimal (default: `"16"`).
        - `const std::string& format`: Format type, either `"fixed"` or `"scientific"` (default: `"fixed"`).
        - `const std::string& alignment`: Alignment type, either `"left"`, `"right"`, or `""` (default: `""`).
        - `const std::string& header`: Optional header line to write at the beginning of the file.
        - `const std::string& footer`: Optional footer line to write at the end of the file.
        - `bool append`: If `true`, append to file instead of overwriting (default: `false`).
        - `size_t col_width`: Column width for alignment (default: `0`, no width constraint).
        - `bool skip_trailing_separator`: If `true`, do not write separator after last value in row (default: `false`).
        - `bool binary`: If `true`, write as raw binary format (default: `false`).

        **Output:** The function does not return any value; it writes the matrix to a file at the specified location.

        **Purpose:** To write a 2D vector (matrix) to a file with flexible formatting options, supporting both text and binary formats, with customizable separators, precision, alignment, and optional headers/footers.

        **Implementation:**

        ```C++
            template <typename t>
            void write_matrix_to_file(
                const Vec<Vec<t>>& matrix, 
                const std::string& directory, 
                const std::string& filename, 
                const std::string& filetype                = "txt", 
                const std::string& separator               = " ", 
                const std::string& comment                 = "", 
                const std::string& precision               = "16", 
                const std::string& format                  = "fixed", 
                const std::string& alignment               = "", 
                const std::string& header                  = "", 
                const std::string& footer                  = "",
                bool               append                  = false,
                size_t             col_width               = 0,
                bool               skip_trailing_separator = false,
                bool               binary                  = false
            )
            {
                std::ios_base::openmode mode = std::ios::out | (append ? std::ios::app : std::ios::trunc);
                if (binary) mode |= std::ios::binary;
                std::ofstream file(directory+"/"+filename+"."+filetype, mode);
                if (!file.is_open())
                {
                    throw std::runtime_error("Failed to open file: " + directory + "/" + filename + "." + filetype);
                }
                if (binary) 
                {
                    // Write matrix dimensions first
                    size_t rows = matrix.size();
                    size_t cols = rows ? matrix[0].size() : 0;
                    file.write(reinterpret_cast<const char*>(&rows), sizeof(size_t));
                    file.write(reinterpret_cast<const char*>(&cols), sizeof(size_t));
                    for (const auto& row : matrix) 
                    {
                        file.write(reinterpret_cast<const char*>(row.data()), sizeof(double) * row.size());
                    }
                    file.close();
                    return;
                }
                // Set floating-point format
                if (format == "fixed")
                    file << std::fixed;
                else if (format == "scientific")
                    file << std::scientific;
                // Set precision
                file << std::setprecision(std::stoi(precision));
                // Alignment (left/right/none)
                if (alignment == "left")
                    file << std::left;
                else if (alignment == "right")
                    file << std::right;
                // Header and comment
                if (!header.empty())
                {
                    if (comment.empty())
                    {
                        file << header << std::endl;
                    }
                    else
                    {
                        file << "# " << header << std::endl;
                        file << "## " << comment << std::endl;
                    }
                }
                auto needs_quoting = [&](const std::string& s) {
                    return (separator == "," || separator == "\t") && (s.find(separator) != std::string::npos || s.find('"') != std::string::npos || s.find('\n') != std::string::npos);
                };
                for (const auto& row : matrix)
                {
                    for (size_t i = 0; i < row.size(); ++i)
                    {
                        std::ostringstream oss;
                        if (col_width > 0) oss << std::setw(col_width);
                        oss << row[i];
                        std::string sval = oss.str();
                        if (needs_quoting(sval)) 
                        {
                            std::string quoted = '"' + sval + '"';
                            sval = quoted;
                        }
                        file << sval;
                        if (!(skip_trailing_separator && i == row.size() - 1))
                        {
                            file << separator;
                        }
                    }
                    file << std::endl;
                }
                if (!footer.empty())
                {
                    file << footer << std::endl;
                }
                file.close();
            }
        ```

        **Special Notes:**
        - For **CSV and TSV formats** (when separator is `","` or `"\t"`), values containing the separator or special characters (quotes, newlines) are automatically quoted for proper escaping.
        - For **binary format**, the matrix dimensions (rows and columns) are written first, followed by the raw binary data.
        - The `append` parameter allows accumulating data in a single file across multiple calls.

        **Example (Text Format):**

        ```C++
            dMatrix matrix = {{0.0, -2.05, 1.25},
                             {2.05, 0.0, -2.33},
                             {-1.25, 2.33, 0.0}};
            write_matrix_to_file(matrix, "./data", "output_matrix", "csv", 
                                ",", "", "6", "fixed", "right",
                                "Sample dMatrix", "End of data");
        ```

        This will create a file `./data/output_matrix.csv` with content:

        ```CSV
            # Sample dMatrix
            ## 
              0.000000, -2.050000, 1.250000
              2.050000,  0.000000,-2.330000
             -1.250000,  2.330000, 0.000000
            End of data
        ```

        **Example (Binary Format):**

        ```C++
            dMatrix matrix = {{1.0, 2.0, 3.0},
                             {4.0, 5.0, 6.0}};
            write_matrix_to_file(matrix, "./data", "binary_matrix", "bin", 
                                " ", "", "16", "fixed", "", "", "", 
                                false, 0, false, true);
        ```

        This will create a binary file `./data/binary_matrix.bin` storing the matrix dimensions and data in raw binary format.

    2. **Vector Writing:**

        **Function Name:** `write_vector_to_file`

        **Input:** The function takes multiple inputs:
        - `const Vec<t>& vector`: The vector to write (1D vector of any type `t`).
        - `const std::string& directory`: Output directory path.
        - `const std::string& filename`: Output filename without extension.
        - `const std::string& filetype`: File extension (default: `"txt"`).
        - `const std::string& separator`: Separator between values (default: `" "`).
        - `const std::string& comment`: Optional comment string (added after header).
        - `const std::string& precision`: Number of digits after decimal (default: `"16"`).
        - `const std::string& format`: Format type, either `"fixed"` or `"scientific"` (default: `"fixed"`).
        - `const std::string& alignment`: Alignment type, either `"left"`, `"right"`, or `""` (default: `""`).
        - `const std::string& header`: Optional header line to write at the beginning of the file.
        - `const std::string& footer`: Optional footer line to write at the end of the file.
        - `bool append`: If `true`, append to file instead of overwriting (default: `false`).
        - `size_t col_width`: Column width for alignment (default: `0`, no width constraint).
        - `bool skip_trailing_separator`: If `true`, do not write separator after last value (default: `false`).
        - `bool binary`: If `true`, write as raw binary format (default: `false`).

        **Output:** The function does not return any value; it writes the vector to a file at the specified location.

        **Purpose:** To write a 1D vector to a file with flexible formatting options, supporting both text and binary formats, with customizable separators, precision, alignment, and optional headers/footers.

        **Implementation:**

        ```C++
            template <typename t>
            void write_vector_to_file(
                const Vec<t>&      vector,
                const std::string& directory,
                const std::string& filename,
                const std::string& filetype                = "txt",
                const std::string& separator               = " ",
                const std::string& comment                 = "",
                const std::string& precision               = "16",
                const std::string& format                  = "fixed",
                const std::string& alignment               = "",
                const std::string& header                  = "",
                const std::string& footer                  = "",
                bool               append                  = false,
                size_t             col_width               = 0,
                bool               skip_trailing_separator = false,
                bool               binary                  = false
            )
            {
                std::ios_base::openmode mode = std::ios::out | (append ? std::ios::app : std::ios::trunc);
                if (binary) mode |= std::ios::binary;
                std::ofstream file(directory+"/"+filename+"."+filetype, mode);
                if (!file.is_open())
                {
                    throw std::runtime_error("Failed to open file: " + directory + "/" + filename + "." + filetype);
                }
                if (binary) {
                    // Write vector size first
                    size_t n = vector.size();
                    file.write(reinterpret_cast<const char*>(&n), sizeof(size_t));
                    file.write(reinterpret_cast<const char*>(vector.data()), sizeof(double) * n);
                    file.close();
                    return;
                }
                // Set floating-point format
                if (format == "fixed")
                    file << std::fixed;
                else if (format == "scientific")
                    file << std::scientific;
                // Set precision
                file << std::setprecision(std::stoi(precision));
                // Alignment (left/right/none)
                if (alignment == "left")
                    file << std::left;
                else if (alignment == "right")
                    file << std::right;
                // Header and comment
                if (!header.empty())
                {
                    if (comment.empty())
                    {
                        file << header << std::endl;
                    }
                    else
                    {
                        file << "# " << header << std::endl;
                        file << "## " << comment << std::endl;
                    }
                }
                auto needs_quoting = [&](const std::string& s) {
                    return (separator == "," || separator == "\t") && (s.find(separator) != std::string::npos || s.find('"') != std::string::npos || s.find('\n') != std::string::npos);
                };
                for (size_t i = 0; i < vector.size(); ++i)
                {
                    std::ostringstream oss;
                    if (col_width > 0) oss << std::setw(col_width);
                    oss << vector[i];
                    std::string sval = oss.str();
                    if (needs_quoting(sval)) {
                        std::string quoted = '"' + sval + '"';
                        sval = quoted;
                    }
                    file << sval;
                    if (!(skip_trailing_separator && i == vector.size() - 1))
                        file << separator;
                }
                if (!footer.empty())
                {
                    file << footer << std::endl;
                }
                file.close();
            }
        ```

        **Special Notes:**
        - For **CSV and TSV formats** (when separator is `","` or `"\t"`), values containing the separator or special characters are automatically quoted for proper escaping.
        - For **binary format**, the vector size is written first, followed by the raw binary data.
        - The `append` parameter allows accumulating values in a single file across multiple calls.
        - Unlike matrix output, vector output is written on a single line (with row separators).

        **Example (Space-Separated Text):**

        ```C++
            dVec vector = {0.1, 1.5, 2.33, -0.5};
            write_vector_to_file(vector, "./data", "output_vector", "txt", 
                                " ", "Time series data", "6", "fixed", "right",
                                "Vector Data");
        ```

        This will create a file `./data/output_vector.txt` with content:

        ```text
            # Vector Data
            ##  Time series data
                    0.100000       1.500000       2.330000      -0.500000
        ```

        **Example (CSV Format):**

        ```C++
            dVec vector = {0.1, 1.5, 2.33, -0.5};
            write_vector_to_file(vector, "./data", "output_vector", "csv", 
                                ",", "", "2", "fixed", "", "Time Series");
        ```

        This will create a file `./data/output_vector.csv` with content:

        ```CSV
            # Time Series
            0.10,1.50,2.33,-0.50
        ```

        **Example (Binary Format):**

        ```C++
            dVec vector = {1.1, 2.2, 3.3, 4.4, 5.5};
            write_vector_to_file(vector, "./data", "binary_vector", "bin", 
                                " ", "", "16", "fixed", "", "", "", 
                                false, 0, false, true);
        ```

        This will create a binary file `./data/binary_vector.bin` storing the vector size and data in raw binary format.

## Usage Notes

- **Directory Creation:** Ensure the output directory exists before writing. You may need to create it separately using filesystem utilities.
- **File Overwrite:** By default, existing files are overwritten. Use `append = true` to preserve existing content.
- **Large Data:** For very large matrices/vectors, binary format is more efficient for both file size and I/O performance.
- **Format Combinations:** Different combinations of `format`, `precision`, and `alignment` allow fine control over output appearance. Common combinations:
  - `format = "fixed"`, `precision = "6"`, `alignment = "right"`: Good for readable numerical output
  - `format = "scientific"`, `precision = "12"`, `alignment = "left"`: Good for very large or very small numbers
  - `format = "fixed"`, `precision = "16"`: Suitable for preserving full double-precision accuracy
