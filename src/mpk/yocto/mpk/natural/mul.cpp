#include "yocto/mpk/word2mpn.hpp"
#include "yocto/code/xbitrev.hpp"
#include "yocto/math/complex.hpp"
#include <cmath>

namespace yocto
{

    namespace mpk
    {


        typedef double                real_t;
        typedef math::complex<real_t> cplx_t;


        //______________________________________________________________________
        //
        // simultaneous FFTs
        //______________________________________________________________________
        static inline
        void _xfft(real_t      *data,
                   real_t      *other,
                   const size_t size
                   ) throw()
        {
            assert( data  != NULL );
            assert( other != NULL );
            assert( is_a_power_of_two(size) );

            --data;
            --other;

            //==================================================================
            // bit reversal algorithm
            //==================================================================
            math::xbitrev::run(data, other, size);

            //==================================================================
            // Lanczos-Algorithm
            //==================================================================
            {
                const size_t n    = size << 1;
                size_t       mmax = 2;
                size_t       mln2 = 1;
                while (n > mmax)
                {
                    const size_t istep = mmax << 1;
                    const size_t isln2 = mln2+1;
                    real_t       wtemp = manager::sin_table[isln2]; //sin(0.5*theta);
                    const real_t wsq   = wtemp*wtemp;
                    real_t wpr         = -(wsq+wsq);
                    real_t wpi         = manager::sin_table[mln2]; //sin(theta);
                    real_t wr          = 1.0;
                    real_t wi          = 0.0;

                    for(register size_t m=1; m<mmax; m+=2)
                    {
                        for(register size_t i=m; i<=n; i+=istep)
                        {
                            const size_t       j     = i+mmax;
                            {
                                real_t      *d_i   = data+i;
                                real_t      *d_j   = data+j;
                                const real_t tempr = wr*d_j[0]-wi*d_j[1];
                                const real_t tempi = wr*d_j[1]+wi*d_j[0];

                                d_j[0]  = d_i[0]-tempr;
                                d_j[1]  = d_i[1]-tempi;
                                d_i[0] += tempr;
                                d_i[1] += tempi;
                            }

                            {
                                real_t      *o_i   = other+i;
                                real_t      *o_j   = other+j;
                                const real_t tempr = wr*o_j[0]-wi*o_j[1];
                                const real_t tempi = wr*o_j[1]+wi*o_j[0];

                                o_j[0]  = o_i[0]-tempr;
                                o_j[1]  = o_i[1]-tempi;
                                o_i[0] += tempr;
                                o_i[1] += tempi;
                            }
                        }
                        wr=(wtemp=wr)*wpr-wi*wpi+wr;
                        wi=wi*wpr+wtemp*wpi+wi;
                    }
                    mmax=istep;
                    mln2=isln2;
                }
            }
        }


        //______________________________________________________________________
        //
        // single FFT
        //______________________________________________________________________
        static inline
        void _fft(real_t      *data,
                  const size_t size
                  ) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );

            --data;

            //==================================================================
            // bit reversal algorithm
            //==================================================================
            math::xbitrev::run(data,size);

            //==================================================================
            // Lanczos-Algorithm
            //==================================================================
            {
                const size_t n    = size << 1;
                size_t       mmax = 2;
                size_t       mln2 = 1;
                while (n > mmax) {
                    const size_t istep = mmax << 1;
                    const size_t isln2 = mln2+1;
                    real_t       wtemp = manager::sin_table[isln2]; //sin(0.5*theta);
                    const real_t wsq   = wtemp*wtemp;
                    real_t wpr         = -(wsq+wsq);
                    real_t wpi         = manager::sin_table[mln2]; //sin(theta);
                    real_t wr          = 1.0;
                    real_t wi          = 0.0;

                    for (size_t m=1; m<mmax; m+=2)
                    {
                        for (size_t i=m; i<=n; i+=istep)
                        {
                            real_t      *d_i   = data+i;
                            const size_t j     = i+mmax;
                            real_t      *d_j   = data+j;
                            const real_t tempr = wr*d_j[0]-wi*d_j[1];
                            const real_t tempi = wr*d_j[1]+wi*d_j[0];

                            d_j[0]  = d_i[0]-tempr;
                            d_j[1]  = d_i[1]-tempi;
                            d_i[0] += tempr;
                            d_i[1] += tempi;
                        }
                        wr=(wtemp=wr)*wpr-wi*wpi+wr;
                        wi=wi*wpr+wtemp*wpi+wi;
                    }
                    mmax=istep;
                    mln2=isln2;
                }
            }


        }


        //______________________________________________________________________
        //
        // inverse FFT
        //______________________________________________________________________
        static  inline
        void _ifft(real_t      *data,
                   const size_t size) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );

            --data;

            //==================================================================
            // bit reversal algorithm
            //==================================================================
            math::xbitrev::run(data, size);

            //==================================================================
            // Lanczos-Algorithm
            //==================================================================
            {
                const size_t n    = size << 1;
                size_t       mmax = 2;
                size_t       mln2 = 1;
                while (n > mmax) {
                    const size_t istep = mmax << 1;
                    const size_t isln2 = mln2+1;
                    real_t       wtemp = -manager::sin_table[isln2]; //sin(0.5*theta);
                    const real_t wsq   = wtemp*wtemp;
                    real_t wpr         = -(wsq+wsq);
                    real_t wpi         = -manager::sin_table[mln2];
                    real_t wr          = 1.0;
                    real_t wi          = 0.0;

                    for (size_t m=1; m<mmax; m+=2)
                    {
                        for (size_t i=m; i<=n; i+=istep)
                        {
                            real_t      *d_i   = data+i;
                            const size_t j     = i+mmax;
                            real_t      *d_j   = data+j;
                            const real_t tempr = wr*d_j[0]-wi*d_j[1];
                            const real_t tempi = wr*d_j[1]+wi*d_j[0];

                            d_j[0]  = d_i[0]-tempr;
                            d_j[1]  = d_i[1]-tempi;
                            d_i[0] += tempr;
                            d_i[1] += tempi;
                        }
                        wr=(wtemp=wr)*wpr-wi*wpi+wr;
                        wi=wi*wpr+wtemp*wpi+wi;
                    }
                    mmax=istep;
                    mln2=isln2;
                }
            }
        }


        natural natural::mul(const natural &lhs, const natural &rhs)
        {

            const size_t nl = lhs.size;
            const size_t nr = rhs.size;
            if(nl>0&&nr>0)
            {
                const size_t np = nl+nr;             //-- product size
                natural      P(np, as_capacity );    //-- product value

                //--------------------------------------------------------------
                //-- compute common power of two
                //--------------------------------------------------------------
                const size_t nn = next_power_of_two(np);

                //--------------------------------------------------------------
                //- compute wokspaces
                //--------------------------------------------------------------
                __array_of<cplx_t> L(nn);
                __array_of<cplx_t> R(nn);
                const uint8_t     *l = lhs.byte;
                const uint8_t     *r = rhs.byte;

                //--------------------------------------------------------------
                //-- fill workspaces
                //--------------------------------------------------------------
                for(size_t i=0;i<nl;++i) L[i].re = real_t(l[i]);
                for(size_t i=0;i<nr;++i) R[i].re = real_t(r[i]);

                //--------------------------------------------------------------
                //-- forward
                //--------------------------------------------------------------
                _xfft( &L[0].re, &R[0].re, nn   );

                //--------------------------------------------------------------
                //-- multiply in place, in L
                //--------------------------------------------------------------
                for(size_t i=0;i<nn;++i)
                {
                    L[i] *= R[i];
                }

                //--------------------------------------------------------------
                //-- reverse
                //--------------------------------------------------------------
                _ifft( &L[0].re, nn );

                //--------------------------------------------------------------
                //-- compute
                //--------------------------------------------------------------
                real_t       carry = 0.0;
                uint8_t     *prod  = P.byte;
                const size_t top   = np - 1;

                for( register size_t i=0; i < top; ++i )
                {
                    carry         +=  L[i].re/nn + 0.5;
                    const real_t q = floor( carry / 256.0 );
                    const real_t r = carry - 256.0 * q;
                    prod[i]        = uint8_t(r);
                    carry          = q;
                }
                prod[top] = uint8_t(carry);

                P.update();
                return P;

            }
            else
            {
                return natural(); // zero...
            }
        }


        natural natural:: sqr( const natural &lhs )
        {
            const size_t nl = lhs.size;
            if( nl > 0  )
            {
                const size_t np = nl << 1;             //-- product size
                natural       P( np, as_capacity );    //-- product value
                //--------------------------------------------------------------
                //-- compute power of two
                //--------------------------------------------------------------
                const size_t nn = next_power_of_two(np);

                //--------------------------------------------------------------
                //- compute wokspace size and create it
                //--------------------------------------------------------------
                __array_of<cplx_t> L( nn );
                const uint8_t   *l = lhs.byte;
                for(size_t i=0;i<nl;++i) L[i].re = real_t(l[i]);

                //--------------------------------------------------------------
                //-- forward
                //--------------------------------------------------------------
                _fft( & L[0].re,  nn  );

                //--------------------------------------------------------------
                //-- multiply in place, in L
                //--------------------------------------------------------------
                for(size_t i=0;i<nn;++i) L[i].in_place_squared();

                //--------------------------------------------------------------
                //-- reverse
                //--------------------------------------------------------------
                _ifft( & L[0].re, nn );

                real_t       carry = 0.0;
                uint8_t     *prod  = P.byte;
                const size_t top   = np - 1;
                for( size_t i=0; i < top; ++i )
                {
                    carry         +=  L[i].re/nn + 0.5;
                    const real_t q = floor( carry / 256.0 );
                    const real_t r = carry - 256.0 * q;
                    prod[i]   = uint8_t(r);
                    carry     = q;
                }
                prod[top] = uint8_t(carry);

                P.update();
                return P;
            }
            else
                return natural();
        }


    }

}


namespace yocto
{
    namespace mpk
    {
        YOCTO_MPN_IMPL2(natural,operator*,natural::mul)

        natural & natural:: operator*=(const word_t rhs)
        {
            const word2mpn RHS(rhs);
            natural        ans = mul(*this,RHS.n);
            xch(ans);
            return *this;
        }

        natural natural::factorial(const natural &n)
        {
            natural ans = 1;
            for(natural i=1;i<=n;++i)
            {
                ans *= i;
            }
            return ans;
        }

        natural natural::factorial(const word_t n)
        {
            natural ans = 1;
            for(word_t i=1;i<=n;++i)
            {
                ans *= i;
            }
            return ans;
        }


    }
}
