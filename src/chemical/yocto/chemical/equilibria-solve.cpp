#include "yocto/chemical/equilibria.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include <iostream>
#include "yocto/code/ipower.hpp"
#include "yocto/code/utils.hpp"


namespace yocto
{
    using namespace math;
    
    namespace chemical
    {
        typedef math::algebra<double> mkl;
        
        bool equilibria:: normalize_C( double t )
        {
            
            const size_t N = nu.rows;
            const size_t M = nu.cols;
            
#if !defined(NDEBUG)
            for(size_t i=M;i>0;--i) { assert(C[i]>=0); }
#endif
            
            if(N>0)
            {
                
                size_t count = 0;
            NEWTON_STEP:
                ++count;
                if(count>MAX_NEWTON_STEPS)
                {
                    std::cerr << "-- Newton-I: not converged" << std::endl;
                    return false;
                }
                //______________________________________________________________
                //
                // compute the Newton's step -dC
                //______________________________________________________________
                if(!compute_Gamma_and_W(t,false))
                {
                    std::cerr << "-- Newton-I: invalid composition" << std::endl;
                    return false;
                }
                mkl::set(xi,Gamma);
                LU.solve(W, xi);
                mkl::mul_trn(dC,nu,xi);
                
                //______________________________________________________________
                //
                // careful subtraction
                //______________________________________________________________
                double shrink = 1;
                bool   cut    = false;
                for( size_t i=M;i>0;--i)
                {
                    const double dd = dC[i];
                    const double cc =  C[i];
                    if(dd>cc)
                    {
                        assert(dd>=0);
                        const double cc_new = cc/2;
                        const double dd_new = cc - cc_new;
                        shrink = min_of(shrink,dd_new/dd);
                        cut = true;
                    }
                }
                mkl::mulsub(C, shrink, dC);
                
                if(cut)
                {
                    std::cerr << "dC=" << dC << std::endl;
                    std::cerr << "shrink=" << shrink << std::endl;
                    goto NEWTON_STEP;
                }
                
                //______________________________________________________________
                //
                // convergence: test full dC
                //______________________________________________________________
                for(size_t i=M;i>0;--i)
                {
                    double err = fabs(dC[i]);
                    if(err<=numeric<double>::tiny)
                        err = 0;
                    if(err> ftol * fabs(C[i]))
                        goto NEWTON_STEP;
                }
                
                //______________________________________________________________
                //
                // check error
                //______________________________________________________________
                if(!compute_Gamma_and_W(t,false))
                {
                    std::cerr << "-- Newton-I: invalid final composition" << std::endl;
                    return false;
                }
                mkl::set(xi,Gamma);
                LU.solve(W, xi);
                mkl::mul_trn(dC,nu,xi);
                for_each( dC.begin(), dC.end(), numeric<double>::round_error);
                for(size_t i=M;i>0;--i)
                {
                    if( C[i] < dC[i] ) C[i] = 0;
                }
                std::cerr << "C=" << C  << std::endl;
                
            }
            return true;
        }
        
        
    }
    
}

