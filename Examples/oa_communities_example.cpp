#include "../includes/typedefs/header.hpp"
#include "../includes/solvers/solver.hpp"
#include "../includes/models/ott-antonsen.dim-red/oa-basic.hpp"
#include "../includes/models/ott-antonsen.dim-red/OA-expanded.hpp"
#include "../includes/models/ott-antonsen.dim-red/OA-helper.hpp"
#include <iostream>
#include <random>
#include <cmath>

using namespace std;

int main()
{
    // small example with C communities
    const size_t C = 2;

    // OA parameters
    dMatrix K(C, dVec(C, 0.1));
    double inter_K0 = 2.001;
    double intra_K0 = 2.001;
    double inter_dK = 0.004;
    double intra_dK = 0.004;
    size_t inter_Kn = 500;
    size_t intra_Kn = 500;

    dMatrix gammas    = { { 0.05, 0.05}, { 0.05, 0.10}, { 0.10, 0.10}};
    dMatrix mus       = {{-0.2,-0.2},{-0.2,0.2},{0.0,0.0},{0.2,0.2}};
    dMatrix couplings = { { intra_K0, inter_K0}, { inter_K0, intra_K0}};


    size_t seed = 123;

    ott_antonsen params{ gammas[0], mus[0], couplings};

    // --- Build initial state for complex form (Re/Im interleaved) ---
    std::mt19937_64 rng(seed);
    std::uniform_real_distribution<double> urho(0.01, 0.2);
    std::uniform_real_distribution<double> uphi(-PI, PI);

    dVec init_rhophi(2*C, 0.0);
    dVec init_reim(2*C, 0.0);
    for (size_t i = 0; i < C; ++i)
    {
        double rho = urho(rng);
        double phi = uphi(rng);
        init_rhophi[2*i + 0] = rho;
        init_rhophi[2*i + 1] = phi;
        init_reim[2*i + 0] = rho * std::cos(phi);
        init_reim[2*i + 1] = rho * std::sin(phi);
    }

    // Solver params for complex form
    SolverParameters sp;
    sp.initial_conditions = init_reim;
    sp.t0 = 0.0;
    sp.t1 = 1.0;
    sp.dt = 0.01;
    sp.derivative = [&](double t, const dVec& y) {
        return ott_antonsen_basic(t, y, params);
    };

    auto res_cplx = rk4_solver(sp);

    // print final magnitudes and phases
    const dVec& final_c = res_cplx.solution.back();
    cout << "Complex-form final (rho,phi):\n";
    for (size_t i = 0; i < C; ++i)
    {
        double re = final_c[2*i + 0];
        double im = final_c[2*i + 1];
        double rho = std::hypot(re, im);
        double phi = std::atan2(im, re);
        cout << "  c=" << i << ": rho=" << rho << ", phi=" << phi << "\n";
    }

    // Rho-Phi
    SolverParameters sp2;
    sp2.initial_conditions = init_rhophi;
    sp2.t0 = 0.0;
    sp2.t1 = sp.t1;
    sp2.dt = 0.01;
    sp2.derivative = [&](double t, const dVec& y)
    {
        return ott_antonsen_expanded(t, y, params);
    };

    auto res_real = rk4_solver(sp2);
    const dVec& final_r = res_real.solution.back();
    cout << "\nReal-form final (rho,phi):\n";
    for (size_t i = 0; i < C; ++i)
    {
        double rho = final_r[2*i + 0];
        double phi = final_r[2*i + 1];
        cout << "  c=" << i << ": rho=" << rho << ", phi=" << phi << "\n";
    }

    // Convert complex-form final to rhophi via helper and compare
    auto rhophi_from_reim = state_to_rhophi_from_reim(final_c);
    cout << "\nConverted complex->(rho,phi):\n";
    for (size_t i = 0; i < C; ++i)
    {
        cout << "  c=" << i << ": rho=" << rhophi_from_reim[i].first << ", phi=" << rhophi_from_reim[i].second << "\n";
    }

    return 0;
}
