#include "yocto/chemical/equilibria.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace chemical
    {
        typedef math::algebra<double> mkl;
        
        bool  equilibria:: normalize( double t, array<double> &C )
        {
            
            //__________________________________________________________________
            //
            // do we need to normalize ?
            //__________________________________________________________________
            if(N<=0)
                return true;
            
            //__________________________________________________________________
            //
            // OK, let us validate at first
            //__________________________________________________________________
            if(!validate(C))
            {
                std::cerr << "Newton-I: invalid set of initial concentrations" << std::endl;
                return false;
            }
            
            //__________________________________________________________________
            //
            // Now we start from a valid set of concentration: initialize
            // first step
            //__________________________________________________________________
            computeGammaAndPhi(t,C);
            std::cerr << "K=" << K << std::endl;
            std::cerr << "C=" << C << std::endl;
            for(size_t count=1;/* count<=20 */;++count)
            {
                //______________________________________________________________
                //
                // compute the full Newton's extent
                //______________________________________________________________
                std::cerr << "Gamma=" << Gamma << std::endl;
                mkl::mul_rtrn(W, Phi, Nu);
                if(!LU.build(W))
                {
                    return false;
                }
                mkl::neg(xi, Gamma);
                lu_t::solve(W, xi);
                std::cerr << "xi_full=" << xi << std::endl;
               
                //______________________________________________________________
                //
                // shall clip xi to control
                //______________________________________________________________
                find_limits_of(C);
                clip_extents();
                std::cerr << "xi_clip=" << xi << std::endl;
                
                //______________________________________________________________
                //
                // find dC
                //______________________________________________________________
                mkl::mul_trn(dC, Nu, xi);
                
                //______________________________________________________________
                //
                // add carefully: MUST be >=0, since xi is clipped
                //______________________________________________________________
                bool converged = true;
                
                for(size_t j=M;j>0;--j)
                {
                    if(active[j]>0)
                    {
                        const double Cj = C[j];
                        const double cc = fabs( (C[j]  = max_of<double>(0.0, Cj+dC[j])) );
                        const double dd = fabs( (dC[j] = C[j] - Cj) );
                        if( dd > fabs(math::numeric<double>::ftol * cc) )
                        {
                            converged = false;
                        }
                    }
                    else
                    {
                        assert(fabs(dC[j])<=0);
                    }
                }
                std::cerr << "C1=" << C << std::endl;
                std::cerr << "dC=" << dC << std::endl;
                if(converged)
                {
                    std::cerr << "converged in " << count << " step" << (count>1? "s" : "") << std::endl;
                    break;
                }
                updateGammaAndPhi(C);
            }
            
            return true;
        }
        
    }
}
