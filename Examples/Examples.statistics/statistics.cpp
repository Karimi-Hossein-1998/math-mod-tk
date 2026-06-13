#include "../../includes/statistics/statistics.hpp"
#include "../../includes/utility/print.hpp"

int main()
{
    dMatrix data = {{1.0, 1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0, 3.0, 4.0, 4.0, 4.0, 5.0, 5.0, 5.0, 6.0, 6.0},
                    {1.0, 1.0, 1.0, 2.0, 2.0, 2.0, 2.0, 2.0, 2.0, 3.0, 3.0, 3.0, 3.0, 4.0, 4.0, 5.0, 5.0, 6.0, 6.0, 6.0},
                    {0.0, 1.0, 2.0, 2.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 4.0, 4.0, 4.0, 5.0, 5.0, 5.0, 5.0, 6.0, 7.0, 8.0}};
    
    dMatrix stats0 = statistics(data, 0);
    dMatrix stats1 = statistics(data, 1);

    std::cout << "Stats in  the [ Mean, Standard Deviance, Coefficient of Variability]\n";
    std::cout << "Along the rows (stats of each rows)\n";
    print_matrix(stats0);
    std::cout << "Along the columns (stats of each column)\n";
    print_matrix(stats1);
}

// > g++ -O3 -std=c++23 statistics.cpp -o statistics.exe
// > ./statistics.exe