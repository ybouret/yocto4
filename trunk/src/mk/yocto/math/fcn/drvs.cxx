#include "yocto/math/ztype.hpp"
#include "yocto/math/fcn/drvs.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/math/round.hpp"
#include <cerrno>

namespace yocto
{
    namespace math
    {
        
#define CON  REAL(1.4)
#define CON2 (CON*CON)
#define NTAB 10
#define SAFE REAL(2.0)
#define BIG  (REAL_MAX/SAFE)
        
#define EVAL()  ( (func(x+hh)-func(x-hh))/(hh+hh) )
        
        template <>
        real_t derivative<real_t>::diff(numeric<real_t>::function &func, real_t x, real_t h, real_t &err)
        {
            
            real_t hh = Fabs(h);
            assert(hh>0);
            
            real_t ans = (a[1][1]=EVAL());
            err=BIG;
            for(size_t i=2;i<=NTAB;i++)
            {
                hh /= CON;
                a[1][i]=EVAL();
                real_t fac=CON2;
                const size_t im = i-1;
                for (size_t j=2;j<=i;j++)
                {
                    const size_t jm = j-1;
                    a[j][i]=(a[jm][i]*fac-a[jm][im])/(fac-REAL(1.0));
                    fac=CON2*fac;
                    const real_t errt=max_of(Fabs(a[j][i]-a[jm][i]),Fabs(a[j][i]-a[jm][im]));
                    if (errt <= err)
                    {
                        err=errt;
                        ans=a[j][i];
                    }
                }
                
                if (Fabs(a[i][i]-a[im][im]) >= SAFE*err)
                {
                    break;
                }
            }
            
            return ans;
        }
        
        
        
        static inline void __eval(real_t                    &dFdx,
                                  real_t                    &d2Fdx2,
                                  numeric<real_t>::function &func,
                                  real_t                     x,
                                  const real_t               Fx,
                                  real_t                     hh)
        {
            const real_t Fp = func(x+hh);
            const real_t Fm = func(x-hh);
            dFdx   = (Fp-Fm)/(hh+hh);
            d2Fdx2 = ((Fp-Fx)+(Fm-Fx))/(hh*hh);
        }
        
        template <>
        void derivative<real_t>::diff(real_t                    &dFdx,
                                      real_t                    &d2Fdx2,
                                      numeric<real_t>::function &func,
                                      real_t                     x,
                                      const real_t               Fx,
                                      real_t                     h,
                                      real_t                    &err)
        {
            real_t hh = Fabs(h);
            assert(hh>0);
            
            __eval(a[1][1],b[1][1],func,x,Fx,hh);
            real_t ans1 = a[1][1];
            real_t ans2 = b[1][1];
            
            err=BIG;
            for(size_t i=2;i<=NTAB;i++)
            {
                hh /= CON;
                __eval(a[1][i], b[1][i], func, x, Fx, hh);
                real_t fac=CON2;
                const size_t im = i-1;
                for (size_t j=2;j<=i;j++)
                {
                    const size_t jm = j-1;
                    const real_t den = fac-REAL(1.0);
                    a[j][i]=(a[jm][i]*fac-a[jm][im])/den;
                    b[j][i]=(b[jm][i]*fac-b[jm][im])/den;
                    fac=CON2*fac;
                    const real_t errt=max_of(Fabs(a[j][i]-a[jm][i]),Fabs(a[j][i]-a[jm][im]));
                    if (errt <= err)
                    {
                        err=errt;
                        ans1=a[j][i];
                        ans2=b[j][i];
                    }
                }
                
                if (Fabs(a[i][i]-a[im][im]) >= SAFE*err)
                {
                    break;
                }
            }

            dFdx   = ans1;
            d2Fdx2 = ans2;
            
        }
        
        
        template <>
        derivative<real_t>:: derivative() :
        a(NTAB,NTAB),
        b(NTAB,NTAB),
        dtol( log_round_ceil( numeric<real_t>::sqrt_epsilon) ),
        hopt( log_round_ceil(Pow( numeric<real_t>::epsilon,REAL(1.0)/REAL(3.0))) ),
        hmin( log_round_ceil(numeric<real_t>::epsilon) )
        {
            //std::cerr << "dtol=" << dtol << "/" << numeric<real_t>::sqrt_epsilon << std::endl;
            //std::cerr << "hopt=" << hopt << std::endl;
            //std::cerr << "hmin=" << hmin << std::endl;
        }
        
        template <>
        derivative<real_t>:: ~derivative() throw()
        {
            
        }
        
        
        template <>
        real_t derivative<real_t>:: operator()(  numeric<real_t>::function &F, real_t x, real_t h )
        {
            volatile real_t temp = 0;
            h = Fabs(h);
            temp = x+h;
            h    = temp-x;
            if(h<=0)
                throw libc::exception( EDOM, "underflow in first derivative" );
            
            // initialize with a first trial
            real_t err = 0;
            real_t dFdx = diff(F, x, h, err);
            while( err > dtol * Fabs(dFdx) )
            {
                real_t new_err = 0;
                h /= CON;
                const real_t new_dFdx = diff(F,x,h,new_err);
                if(new_err>=err)
                    break;
                err  = new_err;
                dFdx = new_dFdx;
            }
            
            return dFdx;
            
        }
        
        template <>
        void derivative<real_t>:: operator()(real_t &dFdx, real_t &d2Fdx2, numeric<real_t>::function &F, real_t x, const real_t Fx, real_t h)
        {
            volatile real_t temp = 0;
            h = Fabs(h);
            temp = x+h;
            h    = temp-x;
            if(h<=0)
                throw libc::exception( EDOM, "underflow in second derivative" );
            
            // initialize with a first trial
            real_t err = 0;
            diff(dFdx, d2Fdx2, F, x, Fx, h, err);
            while( err > dtol * Fabs(dFdx) )
            {
                real_t new_err = 0;
                h /= CON;
                real_t new_dFdx   = 0;
                real_t new_d2Fdx2 = 0;
                diff(new_dFdx, new_d2Fdx2, F, x, Fx, h, new_err);
                if(new_err>=err)
                    break;
                err    = new_err;
                dFdx   = new_dFdx;
                d2Fdx2 = new_d2Fdx2;
            }
            

        }

        
        
    }
}
