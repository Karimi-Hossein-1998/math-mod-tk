#include "../../includes/interpolators/newton-interpolator.hpp"

int main()
{
    dVec         x = {0.0,1.0,2.0,3.0,4.0,5.0,6.0};
    dVec         y = {0.1,0.9,2.1,2.9,4.1,4.9,5.1};

    double  x_star = 4.5;

    func estimator = newton_interpolator(x,y);
    std::cout << "Estimation for single-valued function at point x* = " << x_star << " is:\n" << estimator(x_star) << "\n\n";

    dMatrix Y = {{0.1,0.9,2.1,2.9,4.1,4.9,5.1},
                {0.01,0.8,4.2,8.5,16.9,24.1,26.0},
                {-0.1,0.9,-2.1,2.9,-4.1,4.9,-5.1}};
    
    Func Estimator = newton_interpolator(x,Y);
    
    dVec y_star = Estimator(x_star);
    std::cout << "Estimation for vector-valued function at point x* = " << x_star << " is:\n\n" 
              << "[ " << y_star[0] << ", " << y_star[1] << ", " << y_star[2] << " ]\n";

    return 0;
}

// > g++ -O3 -std=c++23 newton.cpp -o newton.exe
// > ./newton.exe
