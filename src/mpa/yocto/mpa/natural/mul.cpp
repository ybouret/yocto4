#include "yocto/mpa/word2mpn.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/utils.hpp"

namespace yocto
{
    using namespace math;
    
    namespace mpa
    {
        typedef double          real_t;
        typedef complex<real_t> cplx_t;
        
        
        //! simultaneous FFTs
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
            const size_t n    = size << 1;
            {
                register size_t j=1;
                for(register size_t i=1; i<n; i+=2)
                {
                    if(j>i)
					{
                        core::bswap<2*sizeof(real_t)>( data+i,  data+j );
                        core::bswap<2*sizeof(real_t)>( other+i, other+j);
                    }
                    register size_t m = size; // m=  n / 2;
                    while (m >= 2 && j > m)
					{
                        j -= m;
                        m >>= 1;
                    }
                    j += m;
                }
            }
			
            //==================================================================
            // Lanczos-Algorithm
            //==================================================================
            {
                size_t              mmax       = 2;
                size_t              mln2       = 1;
                while (n > mmax)
                {
                    const size_t istep = mmax << 1;
                    const size_t isln2 = mln2+1;
                    real_t       wtemp = memIO::sin_table[isln2]; //sin(0.5*theta);
                    const real_t wsq   = wtemp*wtemp;
                    real_t wpr         = -(wsq+wsq);
                    real_t wpi         = memIO::sin_table[mln2]; //sin(theta);
                    real_t wr          = 1.0;
                    real_t wi          = 0.0;
                    
                    for (size_t m=1; m<mmax; m+=2)
                    {
                        for (size_t i=m; i<=n; i+=istep)
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
            const size_t n = size << 1;
            {
                size_t j=1;
                for (size_t i=1; i<n; i+=2) {
                    if (j > i)
                    {
                        core::bswap<2*sizeof(real_t)>( data+i, data+j);
                    }
                    size_t m = size; // m=  n / 2;
                    while (m >= 2 && j > m) {
                        j -= m;
                        m >>= 1;
                    }
                    j += m;
                }
            }
            
            //==================================================================
            // Lanczos-Algorithm
            //==================================================================
            {
                size_t              mmax       = 2;
                size_t              mln2       = 1;
                while (n > mmax) {
                    const size_t istep = mmax << 1;
                    const size_t isln2 = mln2+1;
                    real_t       wtemp = memIO::sin_table[isln2]; //sin(0.5*theta);
                    const real_t wsq   = wtemp*wtemp;
                    real_t wpr         = -(wsq+wsq);
                    real_t wpi         = memIO::sin_table[mln2]; //sin(theta);
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
        
        static  inline
        void _ifft(real_t      *data,
                   const size_t size
                   ) throw()
        {
            assert( data != NULL );
            assert( is_a_power_of_two(size) );
            
            --data;
            
            //==================================================================
            // bit reversal algorithm
            //==================================================================
            const size_t n = size << 1;
            {
                size_t j=1;
                for (size_t i=1; i<n; i+=2) {
                    if (j > i)
                    {
                        core::bswap<2*sizeof(real_t)>( data+i, data+j);
                    }
                    size_t m = size; // m=  n / 2;
                    while (m >= 2 && j > m) {
                        j -= m;
                        m >>= 1;
                    }
                    j += m;
                }
            }
            
            //==================================================================
            // Lanczos-Algorithm
            //==================================================================
            {
                size_t              mmax       = 2;
                size_t              mln2       = 1;
                while (n > mmax) {
                    const size_t istep = mmax << 1;
                    const size_t isln2 = mln2+1;
                    real_t       wtemp = -memIO::sin_table[isln2]; //sin(0.5*theta);
                    const real_t wsq   = wtemp*wtemp;
                    real_t wpr         = -(wsq+wsq);
                    real_t wpi         = -memIO::sin_table[mln2];
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
        
        natural natural:: mul( const natural &lhs, const natural &rhs )
        {
            const size_t nL = lhs.size;
            const size_t nR = rhs.size;
            if( nL >0 && nR > 0 )
            {
                const size_t nP = nL + nR;             //-- product size
                natural       P( nP, as_capacity );    //-- product value
                
                //--------------------------------------------------------------
                //-- compute common power of two
                //--------------------------------------------------------------
                const size_t nN = next_power_of_two(nP);
                
                //--------------------------------------------------------------
                //- compute wokspaces
                //--------------------------------------------------------------
                array_of<cplx_t> L(nN);
                array_of<cplx_t> R(nN);
                const uint8_t   *l = lhs.byte;
                const uint8_t   *r = rhs.byte;
                
                //--------------------------------------------------------------
                //-- fill workspaces
                //--------------------------------------------------------------
                for(size_t i=0;i<nL;++i) L[i].re = real_t(l[i]);
                for(size_t i=0;i<nR;++i) R[i].re = real_t(r[i]);
                
                //--------------------------------------------------------------
                //-- forward
                //--------------------------------------------------------------
                _xfft( &L[0].re, &R[0].re, nN   );
                
                //--------------------------------------------------------------
                //-- multiply in place, in L
                //--------------------------------------------------------------
                for(size_t i=0;i<nN;++i)
                {
                    L[i] *= R[i];
                }
                
                //--------------------------------------------------------------
                //-- reverse
                //--------------------------------------------------------------
                _ifft( &L[0].re, nN );
                
                real_t       carry = 0.0;
                uint8_t     *prod  = P.byte;
                const size_t top   = nP - 1;
                
                static const real_t half = 0.5;
                for( size_t i=0; i < top; ++i )
                {
                    carry         +=  L[i].re/nN + half;
                    const real_t q = size_t( carry / 256.0 );
                    const real_t r = carry - 256.0 * q;
                    prod[i]   = uint8_t(r);
                    carry     = q;
                }
                prod[top] = uint8_t(carry);
                
                P.rescan();
                return P;
            }
            else
                return natural();
        }
        
        natural operator*( const natural &lhs, const natural &rhs)
        {
            return natural::mul(lhs,rhs);
        }
        
        natural & natural:: operator*=( const natural &rhs)
        {
            natural prod( mul(*this,rhs) );
            xch(prod);
            return *this;
        }
        
        natural natural:: sqr( const natural &lhs )
        {
            const size_t nL = lhs.size;
            if( nL > 0  )
            {
                const size_t nP = nL << 1;             //-- product size
                natural       P( nP, as_capacity );    //-- product value
                //--------------------------------------------------------------
                //-- compute power of two
                //--------------------------------------------------------------
                const size_t nN = next_power_of_two(nP);
                
                //--------------------------------------------------------------
                //- compute wokspace size and create it
                //--------------------------------------------------------------
                array_of<cplx_t> L( nN );
                const uint8_t   *l = lhs.byte;
                for(size_t i=0;i<nL;++i) L[i].re = real_t(l[i]);
                
                //--------------------------------------------------------------
                //-- forward
                //--------------------------------------------------------------
                _fft( & L[0].re,  nN   );
                
                //--------------------------------------------------------------
                //-- multiply in place, in L
                //--------------------------------------------------------------
                for(size_t i=0;i<nN;++i) L[i] *= L[i];
                
                //--------------------------------------------------------------
                //-- reverse
                //--------------------------------------------------------------
                _ifft( & L[0].re, nN );
                
                real_t       carry = 0;
                uint8_t     *prod  = P.byte;
                const size_t top   = nP - 1;
                for( size_t i=0; i < top; ++i )
                {
                    carry         +=  L[i].re/nN + 0.5;
                    const real_t q = size_t( carry / 256.0 );
                    const real_t r = carry - 256.0 * q;
                    prod[i]   = uint8_t(r);
                    carry = q;
                }
                prod[top] = uint8_t(carry);
                
                P.rescan();
                return P;
            }
            else
                return natural();
        }
        
        natural natural:: mul( const natural &lhs, const uint64_t x)
        {
            const word2mpn w(x);
            return mul(lhs,w.n);
        }
        
        natural operator*( const natural &lhs, const uint64_t rhs)
        {
            return natural::mul(lhs,rhs);
        }
        
        natural operator*( const uint64_t lhs, const natural &rhs)
        {
            return natural::mul(rhs,lhs);
        }
        
        natural & natural:: operator*=( const uint64_t rhs)
        {
            natural prod( mul(*this,rhs) );
            xch(prod);
            return *this;
        }
        
        
    }
    
}

