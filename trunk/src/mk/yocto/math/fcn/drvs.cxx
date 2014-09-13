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
        
#if 0
        static inline real_t __fix_drvs( const real_t x, real_t h)
        {
            volatile real_t temp = x+Fabs(h);
            while( (h = (temp-x) ) <= 0 )
            {
                
            }
            return h;
        }
#endif

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
        
        template <>
        derivative<real_t>:: derivative() :
        a(NTAB,NTAB),
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
                throw libc::exception( EDOM, "underflow in derivative" );
            
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
        
        
    }
}
