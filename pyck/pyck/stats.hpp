#ifndef PYCK_STATS_INCLUDED
#define PYCK_STATS_INCLUDED

#include "./c_array.hpp"
#include <cmath>

struct Stats
{
    //! Corrected Two Passes Algorithm
    template<typename T> static inline
    void AverageAndStdErr( double &ave, double &sig, const T *data, size_t n ) throw()
    {
        assert(data);
        assert(n>1);
        double sum = 0;
        for( size_t i=0; i < n; ++i )
        {
            sum += double(data[i]);
        }
        ave = sum/n;
        double sum_dx  = 0;
        double sum_dx2 = 0;
        for( size_t i=0; i < n; ++i )
        {
            const double dx = double(data[i]) - ave;
            sum_dx  += dx;
            sum_dx2 += dx*dx;
        }
        const double var = (sum_dx2 - sum_dx*sum_dx/n)/(n-1);
        sig = sqrt(var);
    }
    
    template<typename T> static inline
    void AverageAndStdErr( double &ave, double &sig, const C_Array<T> &arr ) throw()
    {
        AverageAndStdErr(ave,sig,&arr[0],arr.size);
    }
};

#endif
