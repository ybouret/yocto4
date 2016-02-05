#include "yocto/mpk/natural.hpp"

namespace yocto
{
    namespace mpk
    {
        natural natural::gcd(const natural &x, const natural &y)
        {
            if(x.size>0&&y.size>0)
            {
                natural a = x;
                natural b = y;
                if( b > a )
                {
                    a.xch(b);
                }
                while( b > 0 )
                {
                    natural r = modulo(a,b);
                    a.xch(b);
                    b.xch(r);
                    //const natural t = b;
                    //b = a % b;
                    //a = t;
                }
                return a;
            }
            else
            {
                return natural(1);
            }
        }

        natural  natural:: mod_inv( const natural &b, const natural &n )
        {
            natural n0 = n;
            natural b0 = b;
            natural t0 = 0;
            natural t  = 1;
            natural q  = n0/b0;
            natural r  = n0 - q*b0;

            while( r.size>0 )
            {
                const natural lhs = t0;
                const natural rhs = q*t;

                t0 = t;

                if( compare(lhs,rhs) >= 0 )
                {
                    assert(lhs>=rhs);
                    t = ( (lhs-rhs) % n );
                }
                else
                {
                    assert(rhs>lhs);
                    t  = n - ( (rhs-lhs)%n );
                }
                
                n0 = b0;
                b0 = r;
                q  = n0/b0;
                r  = n0 - q * b0;
            }
            // normally an error if b0 != 1
            return t;
        }

    }
}
