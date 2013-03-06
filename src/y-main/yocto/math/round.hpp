#ifndef YOCTO_MATH_ROUND_INCLUDED
#define YOCTO_MATH_ROUND_INCLUDED 1

#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {
        template <typename T>
        T log_round( T a ) throw()
        {
            assert( a > 0 );
            const double a_log = Floor( Log10(a) );
            const double a_one = Floor( a * Pow(10.0,-a_log));
            return a_one * Pow(10.0,a_log);
        }
    }
}

#endif
