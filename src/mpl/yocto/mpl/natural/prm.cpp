#include "yocto/mpl/natural.hpp"
#include "yocto/code/primes16.hpp"

namespace yocto
{
    namespace mpl
    {
        static inline
        bool __is_prime_above_two(const natural &n )
        {

            mpn q;
            for( size_t i=0; i < YOCTO_PRIMES16_COUNT; ++i )
            {
                q = core::primes16::table[i];
                const mpn qsq = natural::sqr(q);
                if( qsq >  n )     return true;
                if( (n % q) == 0 ) return false;
            }

            const natural two(2);
            q = YOCTO_PRIMES16_NEXT;
            while( true /* q * q <= n */ )
            {
                const mpn qsq = natural::sqr(q);
                if(qsq > n ) break;
                if( (n % q) == 0 ) return false;
                q += two;
            }
            return true;
        }

        bool natural:: __is_prime( const natural &n )
        {
            if(n<2)
            {
                return false;
            }
            else
            {
                return __is_prime_above_two(n);
            }
        }

        natural natural:: __next_prime(const natural &n )
        {
            if( n <= 2 )
            {
                return 2;
            }
            else
            {
                const natural two(2);
                mpn q = n;
                if( q.is_even() ) ++q;
                while( ! __is_prime_above_two( q ) )
                {
                    q += two;
                }
                return q;
            }

        }

    }

}
