#ifndef YOCTO_MATH_ROUND_INCLUDED
#define YOCTO_MATH_ROUND_INCLUDED 1

#include "yocto/math/types.hpp"

namespace yocto
{
    namespace math
    {
        template <typename T>
        inline T log_round_floor( T a ) throw()
        {
            assert( a > 0 );
            const T a_log = Floor( Log10(a) );
            const T a_one = Floor( a * Pow( T(10.0),-a_log));
            return a_one * Pow( T(10.0),a_log);
        }
        
        template <typename T>
        inline T log_round_ceil( T a ) throw()
        {
            assert( a > 0 );
            const T a_log = Floor( Log10(a) );
            const T a_one = Ceil( a * Pow( T(10.0),-a_log));
            return  a_one * Pow( T(10.0),a_log);
        }
        
        

        template <typename T>
        inline size_t simulation_save_every( T &dt, T &dt_save )
        {
            assert(dt>0);
            dt = log_round_ceil(dt);
            if(dt_save<=dt) dt_save = dt;
            size_t every = size_t(Floor(dt_save/dt));
            if(every<1) every=1;
            dt_save = every*dt;
            return every;
        }

        template <typename T>
        inline size_t simulation_iter( const T t_run, const T dt, const size_t every )
        {
            assert(dt>0);
            size_t iter = size_t(Ceil(Fabs(t_run/dt)));
            if(iter<every) iter = every;
            while( 0 != (iter%every) ) ++iter;
            return iter;
        }
        
    }
}

#endif
