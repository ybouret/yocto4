#include "erfc.hpp"
#include <cmath>

#define real_t double
#define REAL   double

double Erfc(double x)
{
    const real_t z   = fabs(x);
    const real_t t   = REAL(1.0)/(REAL(1.0)+REAL(0.5)*z);
    const real_t ans = t*exp(-z*z
                             -REAL(1.26551223)
                             +t*(REAL(1.00002368)+t*(REAL(0.37409196)+t*(REAL(0.09678418)+
                                                                                             t*(-REAL(0.18628806)+t*(REAL(0.27886807)+t*(-REAL(1.13520398)+t*(REAL(1.48851587)+
                                                                                                                                                              t*(-REAL(0.82215223)+t*REAL(0.17087277))))))))));
    return (x >= REAL(0.0)) ? ans : REAL(2.0)-ans;
}
