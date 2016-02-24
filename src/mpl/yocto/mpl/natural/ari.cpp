#include "yocto/mpl/natural.hpp"
#include <iostream>

namespace yocto
{
    namespace mpl
    {
        natural natural:: gcd(const natural &x, const natural &y)
        {
            if(x.size>0&&y.size>0)
            {
                natural a = x;
                natural b = y;
                if( b > a )
                {
                    a.xch(b);
                }
                while( b.size>0 )
                {
                    natural r = __mod(a.byte,a.size,b.byte,b.size);
                    a.xch(b);
                    b.xch(r);
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
            //std::cerr << "Computing " << b << "^(-1)[" << n << "]" << std::endl;
            natural n0 = n;
            natural b0 = b;
            natural t0 = 0;
            natural t  = 1;
            natural q  = n0/b0;
            natural r  = n0 - q*b0;

            while( r>0 )
            {
                const natural lhs = t0;
                const natural rhs = q*t;

                t0 = t;

                if( compare(lhs,rhs) >= 0 )
                {
                    assert(lhs>=rhs);
                    t = ( (lhs-rhs) % n );
                    assert(t<n);
                }
                else
                {
                    assert(rhs>lhs);
                    t  = n - ( (rhs-lhs)%n );
                    assert(t<=n);
                }
                
                n0 = b0;
                b0 = r;
                q  = n0/b0;
                r  = n0 - q * b0;
            }
            
            // normally an error if b0 != 1
            //std::cerr << b << "*" << t << "[" << n << "]=" << (b*t)%n << std::endl;
            return t;
        }

        natural natural:: mod_exp( const natural &B, const natural &E, const natural &N )
        {

            if( N.size <= 0 )
                throw libc::exception( EDOM, "mpl.mod_exp %% 0");


            natural result = 1;
            if( E.size > 0 )
            {
                assert( E.bits() > 0 );
                natural        base  = B;
                const size_t   nbit  = E.bits()-1;
                const uint8_t *ebit  = E.byte;

                for( size_t i=0; i < nbit; ++i )
                {

                    if( ebit[i>>3] & _bit[ i & 7 ] )
                    {
                        //result = ( result * base ) % N;
                        const natural rb = result * base;

                        natural tmp1 = __mod( rb.byte, rb.size, N.byte, N.size );
                        tmp1.xch( result );
                    }
                    //base = ( base * base ) % N;
                    const natural bsq  = sqr(base);
                    natural       tmp2 = __mod(bsq.byte,bsq.size,N.byte,N.size);
                    base.xch( tmp2 );
                }

                //-- most significant bit !
                assert( (ebit[nbit>>3] & _bit[ nbit & 7 ]) != 0 );

                //result = ( result * base ) % N;
                const natural rb   = result * base;
                natural       tmp3 = __mod( rb.byte, rb.size, N.byte, N.size );
                tmp3.xch( result );
            }

            return result;
            
        }

        void natural:: simplify(natural &lhs, natural &rhs)
        {
            if(lhs.is_zero())
            {
                if(!rhs.is_zero()) rhs=1;
                return;
            }
            
            if(rhs.is_zero())
            {
                if(!lhs.is_zero()) lhs=1;
                return;
            }
            
            const mpn fac = natural::gcd(lhs,rhs);
            if(!fac.is_byte(1))
            {
                natural L = lhs/fac;
                natural R = rhs/fac;
                L.xch(lhs);
                R.xch(rhs);
            }
        }


    }
}
