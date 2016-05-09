#include "yocto/mpl/natural.hpp"
#include "yocto/code/xbitrev.hpp"
#include "yocto/math/complex.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>
#include <cmath>

namespace yocto
{
    namespace mpl
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

        //! assuming ff1 is zeroed, same sime than fft2
        void _fft_v2(const uint8_t *data1,
                     const size_t   size1,
                     const uint8_t *data2,
                     const real_t   size2,
                     real_t        *fft1,
                     real_t        *fft2,
                     const size_t   n) throw()
        {
            assert( data1 != NULL );
            assert( data2 != NULL );
            assert( fft1  != NULL );
            assert( fft2  != NULL );

            assert( is_a_power_of_two(n) );
            assert(size1<=n);
            assert(size2<=n);

            //__________________________________________________________________
            //
            // packing real data in fft1
            //__________________________________________________________________
#define Y_MPN_SET_FFT_RE(J) fft1[     (J)<<1   ] = data1[J]
#define Y_MPN_SET_FFT_IM(J) fft1[ 1+( (J)<<1 ) ] = data2[J]
            YOCTO_LOOP_FUNC_(size1,Y_MPN_SET_FFT_RE,0);
            YOCTO_LOOP_FUNC_(size2,Y_MPN_SET_FFT_IM,0);

            //__________________________________________________________________
            //
            // forward transform complex fft1
            //__________________________________________________________________
            _fft(fft1,n);

            //__________________________________________________________________
            //
            // decompose fourier transforms
            //__________________________________________________________________
            const size_t np1 = n+1;
            const size_t nn2 = np1<<1;
            const size_t nn3 = nn2+1;
            --fft1;
            --fft2;
            fft2[1]=fft1[2];
            fft1[2]=fft2[2]=0;
            for(size_t j=3;j<=np1;j+=2)
            {
                const size_t jp1= j+1;
                const real_t rep=(0.5)*(fft1[j]   + fft1[nn2-j]);
                const real_t rem=(0.5)*(fft1[j]   - fft1[nn2-j]);
                const real_t aip=(0.5)*(fft1[jp1] + fft1[nn3-j]);
                const real_t aim=(0.5)*(fft1[jp1] - fft1[nn3-j]);
                fft1[j]     =  rep;
                fft1[jp1]   =  aim;
                fft1[nn2-j] =  rep;
                fft1[nn3-j] = -aim;
                fft2[j]     =  aip;
                fft2[jp1]   = -rem;
                fft2[nn2-j] =  aip;
                fft2[nn3-j] =  rem;
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


        natural natural::mul(const void *lhs, const size_t nl,
                             const void *rhs, const size_t nr)
        {

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
                array_of<cplx_t> L(nn);
                array_of<cplx_t> R(nn);
                const uint8_t     *l = (const uint8_t *)lhs;
                const uint8_t     *r = (const uint8_t *)rhs;

                //--------------------------------------------------------------
                //-- fill workspaces
                //--------------------------------------------------------------
#define Y_MPN_SET_L(I) L[I].re = real_t(l[I])
#define Y_MPN_SET_R(I) R[I].re = real_t(r[I])
                YOCTO_LOOP_FUNC_(nl,Y_MPN_SET_L,0);
                YOCTO_LOOP_FUNC_(nr,Y_MPN_SET_R,0);

                //--------------------------------------------------------------
                //-- forward
                //--------------------------------------------------------------
                _xfft( &L[0].re, &R[0].re, nn   );

                //--------------------------------------------------------------
                //-- multiply in place, in L
                //--------------------------------------------------------------
#define Y_MPN_MUL_L_BY_R(I) L[I] *= R[I]
                YOCTO_LOOP_FUNC_(nn, Y_MPN_MUL_L_BY_R, 0);

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
#define Y_MPN_MUL_PUT(i)                      \
carry         += L[i].re/nn + 0.5;            \
const real_t q = floor( carry * 0.00390625 ); \
const real_t r = carry - (256.0 * q);         \
prod[i]        = uint8_t(r);                  \
carry          = q;
                YOCTO_LOOP_FUNC_(top,Y_MPN_MUL_PUT,0);
                prod[top] = uint8_t(carry);
                P.update();
                return P;
            }
            else
            {
                return natural(); // zero...
            }
        }

        natural natural::mul_v2(const void *lhs, const size_t nl,
                                const void *rhs, const size_t nr)
        {

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
                array_of<cplx_t> L(nn);
                array_of<cplx_t> R(nn);
                const uint8_t   *l = (const uint8_t *)lhs;
                const uint8_t   *r = (const uint8_t *)rhs;

                //--------------------------------------------------------------
                //-- get coupled fft
                //--------------------------------------------------------------
                _fft_v2(l,nl,r,nr,&L[0].re,&R[0].re,nn);

                //--------------------------------------------------------------
                //-- multiply in place, in L
                //--------------------------------------------------------------
                YOCTO_LOOP_FUNC_(nn, Y_MPN_MUL_L_BY_R, 0);

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
                YOCTO_LOOP_FUNC_(top,Y_MPN_MUL_PUT,0);
                prod[top] = uint8_t(carry);
                
                P.update();
                return P;
            }
            else
            {
                return natural();
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
                array_of<cplx_t> L( nn );
                const uint8_t   *l = lhs.byte;
                YOCTO_LOOP_FUNC_(nl,Y_MPN_SET_L,0);

                //--------------------------------------------------------------
                //-- forward
                //--------------------------------------------------------------
                _fft( & L[0].re,  nn  );

                //--------------------------------------------------------------
                //-- multiply in place, in L
                //--------------------------------------------------------------
#define Y_MPN_SQR_L(I) L[I].in_place_squared()
                YOCTO_LOOP_FUNC_(nn,Y_MPN_SQR_L,0);
                
                //--------------------------------------------------------------
                //-- reverse
                //--------------------------------------------------------------
                _ifft( & L[0].re, nn );
                
                real_t       carry = 0.0;
                uint8_t     *prod  = P.byte;
                const size_t top   = np - 1;
                YOCTO_LOOP_FUNC_(top,Y_MPN_MUL_PUT,0);
                prod[top] = uint8_t(carry);
                
                P.update();
                return P;
            }
            else
                return natural();
        }
        
        
    }
    
}
