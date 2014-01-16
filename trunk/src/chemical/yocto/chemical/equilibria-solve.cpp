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
            
            //__________________________________________________________________
            //
            // We start from a valid composition
            //__________________________________________________________________

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
                //const double H0 = Gamma2RMS();
                mkl::set(xi,Gamma);
                LU.solve(W, xi);
                mkl::mul_trn(dC,nu,xi);
                
                //______________________________________________________________
                //
                // control the step, descent direction for |Gamma"
                //______________________________________________________________
                
                // save C
                mkl::set(CC,C);

                // full step
                mkl::sub(C,dC);
                
#if 0
                double H1 = compute_rms(t);
                
                // control
                double alpha = 1;
                while(H1>H0)
                {
                    alpha *= 0.1;
                    if( alpha < numeric<double>::ftol )
                    {
                        std::cerr << "-- Newton-I: spurious point" << std::endl;
                        goto FINALIZE;
                    }
                    //std::cerr << "alpha=" << alpha << " / H0=" << H0 << " / H1=" << H1 << "/ diff=" << H1-H0 << std::endl;
                    mkl::set(C,CC);
                    mkl::mulsub(C,alpha,dC);
                    H1 = compute_rms(t);
                }
                
#endif
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
                
            FINALIZE:
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
                //std::cerr << "dC=" << dC << std::endl;
                //std::cerr << "C=" << C  << std::endl;
                
            }
            return true;
        }
        
        
    }
    
}

