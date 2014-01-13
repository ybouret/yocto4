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
                
                std::cerr << "Newton@ftol=" << ftol << std::endl;
                
            NEWTON_STEP:
                //______________________________________________________________
                //
                // compute the Newton's step -dC
                //______________________________________________________________
                if(!compute_Gamma_and_W(t,false))
                    return false;
                //const double H0 = Gamma2RMS();
                mkl::set(xi,Gamma);
                LU.solve(W, xi);
                mkl::mul_trn(dC,Nu,xi);
                
                //______________________________________________________________
                //
                // careful subtraction
                //______________________________________________________________
                mkl::set(CC,C);
                std::cerr << "dC0=" <<dC << std::endl;
                bool cut = false;
                for(size_t i=M;i>0;--i)
                {
                    double &dd = dC[i];
                    double &cc = C[i];
                    if(dd>cc)
                    {
                        cc /= 2;
                        dd  = 0;
                        cut = true;
                    }
                }
                
                mkl::sub(C,dC);
#if !defined(NDEBUG)
                for(size_t i=M;i>0;--i) { assert(C[i]>=0); }
#endif
                
                if(cut)
                    goto NEWTON_STEP;
                
                //______________________________________________________________
                //
                // Effective increase
                //______________________________________________________________
                mkl::sub(CC,C);
                
                std::cerr << "Gamma  =" << Gamma << std::endl;
                std::cerr << "dC     =" << CC    << std::endl;
                //std::cerr << "shrink =" << shrink << std::endl;
                std::cerr << "C      =" << C      << std::endl;
                
                //______________________________________________________________
                //
                // convergence
                //______________________________________________________________
               
                
                for(size_t i=M;i>0;--i)
                {
                    double err = fabs(CC[i]);
                    if(err<=numeric<double>::tiny) err = 0;
                    if(err> ftol * fabs(C[i]))
                        goto NEWTON_STEP;
                }
                
                //______________________________________________________________
                //
                // check error
                //______________________________________________________________
                
                if(!compute_Gamma_and_W(t,false))
                    return false;
                mkl::set(xi,Gamma);
                LU.solve(W, xi);
                mkl::mul_trn(dC,Nu,xi);
                for(size_t i=M;i>0;--i)
                {
                    double err = fabs(dC[i]);
                    if(err<=numeric<double>::tiny)
                        err=0;
                    else
                        err = pow(10,ceil(log10(err)));
                    dC[i] = err;
                }
                //std::cerr << "C1  =" << C      << std::endl;
                
                for(size_t i=M;i>0;--i)
                {
                    if( C[i] < dC[i] ) C[i] = 0;
                }
                std::cerr << "C   =" << C  << std::endl;
                //std::cerr << "Cerr=" << dC << std::endl;
                //std::cerr << std::endl;
                
            }
            return true;
        }


    }

}

