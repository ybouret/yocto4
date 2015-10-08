#include "yocto/math/ztype.hpp"
#include "yocto/math/types.hpp"
#include "yocto/math/fft/psd.hpp"
#include "yocto/math/fft/fft1.hpp"

#include "yocto/code/utils.hpp"
#include "yocto/code/unroll.hpp"

#include "yocto/exceptions.hpp"
#include "yocto/sequence/vector.hpp"

#include "yocto/math/kernel/crout.hpp"
#include "yocto/math/core/tao.hpp"

#include "yocto/code/ipower.hpp"

#include <cerrno>

namespace yocto
{
	
    namespace math
    {
		
        template <>
        real_t PSD<real_t>:: Square( real_t X ) throw()
        {
            return X>=REAL(0.0) && X<=REAL(1.0) ? REAL(1.0) : REAL(0.0);
        }
		
        template <>
        real_t PSD<real_t>:: Bartlett( real_t X ) throw()
        {
            return X>REAL(0.0) && X<REAL(1.0) ? REAL(1.0) - Fabs( (X-REAL(0.5) / REAL(0.5) ) ): REAL(0.0);
        }
		
        template <>
        real_t PSD<real_t>:: Hann( real_t X ) throw()
        {
            return X>REAL(0.0) && X<REAL(1.0) ? REAL(0.5) * (REAL(1.0) - Cos( numeric<real_t>::two_pi * X)) : REAL(0.0) ;
        }
		
        template <>
        real_t PSD<real_t>:: Welch( real_t X ) throw()
        {
            if(  X>0 && X<1 )
            {
                const real_t tmp = (X-REAL(0.5) / REAL(0.5));
                return REAL(1.0) - tmp * tmp;
            }
            else
            {
                return 0;
            }
			
        }
		
        
        template<>
        real_t PSD<real_t>:: Blackman( real_t X ) throw()
        {
            if( X > 0 && X < 1 )
            {
                const real_t tmp  = numeric<real_t>::two_pi * X;
                return REAL(0.42) - REAL(0.50) * Cos( tmp  ) + REAL(0.08) * Cos( tmp + tmp );
            }
            else
            {
                return 0;
            }
        }
        
        template <>
        real_t PSD<real_t>:: Nutall( real_t X ) throw()
        {
            if( X > 0 && X < 1 )
            {
                const real_t pi2 = numeric<real_t>:: two_pi * X;
                const real_t pi4 = pi2+pi2;
                const real_t pi6 = pi4+pi2;
                return REAL(0.355768) - REAL(0.487396) * Cos(pi2) + REAL(0.144232) * Cos(pi4) - REAL(0.012604) * Cos(pi6);
            }
            else
                return 0;
        }
		
        
        template <>
        void PSD<real_t>:: Compute(Window        &w,
                                   real_t        *p,
                                   const size_t   m,
                                   const real_t *data,
                                   const size_t  size,
                                   const size_t  K)
        {
            
            assert(p!=NULL);
            assert(m>0);
            assert(is_a_power_of_two(m));
            assert(!(data==NULL&&size>0));
            
            
            //==================================================================
            // initialize data
            //==================================================================
            const size_t M = m << 1;
            if( M > size )
                throw exception("PSD: %u*2 > data size=%u", unsigned(m), unsigned(size));
            
            
            vector<cplx_t> work(  M, numeric<cplx_t>::zero );
            vector<real_t> work2( M, numeric<real_t>::zero );
            vector<real_t> F(M, numeric<real_t>::zero );
            
            cplx_t        *input  = work(0);
            real_t        *weight = work2(0);
            real_t        *f      = F(0);
            
            //==================================================================
            // initialize the weights
            //==================================================================
            real_t         sumw2  = 0;
            {
                const real_t wfac = REAL(1.0) / (M-1);
                for( size_t i=0; i < M; ++i )
                {
                    const real_t tmp = weight[i] = w( i * wfac ); // or (i+0.5)/M ?
                    sumw2 += tmp*tmp;
                }
            }
            
            //==================================================================
            // initialize the spectrum
            //==================================================================
            for(size_t i=0;i<m;++i)
                p[i] = 0;
            
            //==================================================================
            // initialize the filtering
            //==================================================================
            matrix<real_t>  sig;
            vector<real_t>  a;
            if(K>0)
            {
                //std::cerr << "K=" << K << std::endl;
                sig.make(K,M);
                a.make(K, numeric<real_t>::zero );
                
                //--------------------------------------------------------------
                // Build the matrix of moments
                //--------------------------------------------------------------
                matrix<real_t> mu(K,K);
                for( size_t j=1;j<=K;++j)
                {
                    for( size_t k=j;k<=K;++k)
                    {
                        const size_t p = j+k-1;
                        mu[j][k] = mu[k][j] = ipower<real_t>(real_t(M), p) / real_t(p);
                    }
                }
                //std::cerr << "mu=" << mu << std::endl;
                if( !crout<real_t>::build(mu) )
                    throw exception("PSD(Invalid Momemts");
                
                //--------------------------------------------------------------
                // Build sigma matrix, to get coefficients
                //--------------------------------------------------------------
                for(size_t i=1;i<=M;++i)
                {
                    const size_t im = i-1;
                    for(size_t j=1;j<=K;++j)
                    {
                        sig[j][i] = (ipower<real_t>(real_t(i),j)-ipower<real_t>(real_t(im),j))/real_t(j);
                    }
                }
                //std::cerr << "sig0=" << sig << std::endl;
                crout<real_t>::solve(mu, sig);
                //std::cerr << "sigm=" << sig << std::endl;
            }
            
            //==================================================================
            // loop over segments
            //==================================================================
            size_t      nsub =0;
            for( size_t start=0; start+M <= size; ++start)
            {
                
                //--------------------------------------------------------------
                //-- load the sample
                //--------------------------------------------------------------
                for(size_t i=0;i<M;++i)
                {
                    assert(start+i<size);
                    f[i] = data[start+i];
                }
                
                //--------------------------------------------------------------
                //-- compute the coefficients
                //--------------------------------------------------------------
                if(K>0)
                {
                   tao::mul(a, sig, F);
                }
                //std::cerr << "a=" << a <<  std::endl;
                
                //--------------------------------------------------------------
                //-- load the sample
                //--------------------------------------------------------------
                for( size_t i=0; i<M; ++i )
                {
                    //-- compute the drift
                    real_t sum = 0;
                    for(size_t j=K;j>0;--j)
                    {
                        sum += a[j] * ipower<real_t>(real_t(i),j-1);
                    }
                    
                    //-- compute the effective signal
                    real_t f_i = f[i] - sum;
                    
                    //-- put it as a weighted complex
                    cplx_t &g = input[i];
                    g.re = f_i * weight[i];
                    g.im = 0;
                }
                
                //--------------------------------------------------------------
                //-- take the local FFT
                //--------------------------------------------------------------
                FFT(input,M);
                for(size_t i=0;i<m;++i)
                    p[i] += input[i].mod2();
                ++nsub;
            }
            
            //==================================================================
            // normalize
            //==================================================================
            const real_t nfac = REAL(1.0)/(nsub*sumw2);
            for(size_t i=0;i<m;++i)
                p[i] *= nfac;
        }
        
        template <>
        void PSD<real_t>:: Compute(Window &w, array<real_t> &psd, const array<real_t> &data, const size_t K)
        {
            PSD<real_t>::Compute( w, psd(0), psd.size(), data(0), data.size(), K);
        }
        
        
    }
	
}
