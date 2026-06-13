#include <iostream>
#include <fstream>
#include <string>
#include <exception>

int main() 
{
    const std::string filename = "file_write_stress_test_output.bin";
    const size_t block_size = 1024 * 1024; // 1 MB
    const std::string block(block_size, 'A');
    const size_t max_size = 1024ULL * 1024ULL * 1024ULL * 10ULL; // 10 GB (adjust as needed)
    size_t total_written = 0;
    size_t block_count = 0;
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Failed to open file for writing!\n";
        return 1;
    }
    try
    {
        while (total_written < max_size)
        {
            file.write(block.data(), block.size());
            if (!file)
            {
                throw std::ios_base::failure("Write failed (possibly out of space)");
            }
            total_written += block.size();
            ++block_count;
            if (block_count % 100 == 0)
            {
                std::cout << "Written " << (total_written / (1024 * 1024)) << " MB...\n";
            }
        }
        std::cout << "Reached target file size: " << (total_written / (1024 * 1024)) << " MB\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception encountered: " << e.what() << "\n";
        std::cerr << "Total written: " << (total_written / (1024 * 1024)) << " MB\n";
    }
    file.close();
    return 0;
} 