#ifndef YOCTO_MATH_ROUND_INCLUDED
#define YOCTO_MATH_ROUND_INCLUDED 1

#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {
        template <typename T>
        inline T log_round( T a ) throw()
        {
            assert( a > 0 );
            const double a_log = Floor( Log10(a) );
            const double a_one = Floor( a * Pow( T(10.0),-a_log));
            return a_one * Pow( T(10.0),a_log);
        }
     
        template <typename T>
        inline void simulation_times( T &dt, T &dt_save, size_t &every )
        {
            assert(dt>0);
            dt = log_round(dt);
            if(dt_save<=dt) dt_save = dt;
            every   = size_t(Floor(dt_save/dt));
            if(every<1) every=1;
            dt_save = every * dt;
        }
        
    }
}

#endif
