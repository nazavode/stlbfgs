#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <iostream>

#include "linesearch.h"

using namespace STLBFGS;

namespace STLBFGS {
    double find_cubic_minimizer(double a, double fa, double ga, double b, double fb, double gb);
    double find_quadratic_minimizer(double a, double fa, double ga, double b, double fb);
    double find_quadratic_minimizer(double a, double ga, double b, double gb);
}

TEST_CASE("cubic 1", "[interpolation]") {
    auto f = [](const double a) { return a*a*a - a; };
    auto g = [](const double a) { return 3*a*a - 1; };
    double a = -.4;
    double b = 19.;
    double minimizer = find_cubic_minimizer(a, f(a), g(a), b, f(b), g(b));
    REQUIRE( std::abs(minimizer-1./std::sqrt(3.))<1e-14 );
}

TEST_CASE("cubic 2", "[interpolation]") {
    auto f = [](const double a) { return a*a*a - 3*a*a - 144*a + 432; };
    auto g = [](const double a) { return 3*a*a - 6*a - 144; };
    double a = -5.;
    double b = 113.;
    double minimizer = find_cubic_minimizer(a, f(a), g(a), b, f(b), g(b));
    REQUIRE( std::abs(minimizer-8.)<1e-14 );
}

TEST_CASE("quadratic", "[interpolation]") {
    for (int i=0; i<8; i++) {
        double A = rand()/(double)RAND_MAX + .2134;
        double B = rand()/(double)RAND_MAX;
        double C = rand()/(double)RAND_MAX;
        double M = -B/(2.*A);
        auto f = [&](const double a) { return A*a*a + B*a + C; };
        auto g = [&](const double a) { return 2*A*a + B; };
        double a = M - rand()/(double)RAND_MAX;
        double b = M + rand()/(double)RAND_MAX;
        double minimizer1 = find_quadratic_minimizer(a, f(a), g(a), b, f(b));
        REQUIRE( std::abs(minimizer1 - M)<1e-14 );
        double minimizer2 = find_quadratic_minimizer(a, g(a), b, g(b));
        REQUIRE( std::abs(minimizer2 - M)<1e-14 );
    }
}

template <typename T> auto square(const T &number) { return number * number; }

TEST_CASE("Table 1", "[Mor\\'e-Thuente]") {
    std::cerr << "\n\nTable 1\n\n";
    const linesearch_function func = [](const double alpha) -> Sample {
        constexpr double beta = 2.;
        double f = -alpha/(alpha*alpha + beta);
        double g = (square(alpha)-beta)/square(beta+square(alpha));
        return { alpha, f, g };
    };
    double alpha = 1e-3;
    REQUIRE( line_search(func, func(0), alpha, 1e-3, 1e-1) );
    alpha = 1e-1;
    REQUIRE( line_search(func, func(0), alpha, 1e-3, 1e-1) );
    alpha = 1e+1;
    REQUIRE( line_search(func, func(0), alpha, 1e-3, 1e-1) );
    alpha = 1e+3;
    REQUIRE( line_search(func, func(0), alpha, 1e-3, 1e-1) );
}

TEST_CASE("Table 2", "[Mor\\'e-Thuente]") {
    std::cerr << "\n\nTable 2\n\n";
    const linesearch_function func = [](const double alpha) -> Sample {
        constexpr double beta = .004;
        double f = pow(alpha+beta, 5.) - 2.*pow(alpha+beta, 4.);
        double g = 5.*pow(alpha+beta, 4.) - 8.*pow(alpha+beta, 3.);
        return { alpha, f, g };
    };
    double alpha = 1e-3;
    REQUIRE( line_search(func, func(0), alpha, 1e-1, 1e-1) );
    alpha = 1e-1;
    REQUIRE( line_search(func, func(0), alpha, 1e-1, 1e-1) );
    alpha = 1e+1;
    REQUIRE( line_search(func, func(0), alpha, 1e-1, 1e-1) );
    alpha = 1e+3;
    REQUIRE( line_search(func, func(0), alpha, 1e-1, 1e-1) );
}

TEST_CASE("Table 3", "[Mor\\'e-Thuente]") {
    std::cerr << "\n\nTable 3\n\n";
    const linesearch_function func = [](const double alpha) -> Sample {
        constexpr double beta = .01;
        constexpr double l = 39;

        double f = 2.*(1.-beta)/(l*M_PI)*sin(l*M_PI_2*alpha);
        double g = (1.-beta)*cos(l*M_PI_2*alpha);
        if (alpha<=1.-beta) {
            f += 1.-alpha;
            g += -1.;
        } else if (alpha>=1+beta) {
            f += alpha-1.;
            g += 1.;
        } else {
            f += square(alpha-1.)/(2.*beta) + beta/2.;
            g += (alpha-1.)/beta;
        }

        return { alpha, f, g };
    };
    double alpha = 1e-3;
    REQUIRE( line_search(func, func(0), alpha, 1e-1, 1e-1) );
    alpha = 1e-1;
    REQUIRE( line_search(func, func(0), alpha, 1e-1, 1e-1) );
    alpha = 1e+1;
    REQUIRE( line_search(func, func(0), alpha, 1e-1, 1e-1) );
    alpha = 1e+3;
    REQUIRE( line_search(func, func(0), alpha, 1e-1, 1e-1) );
}


