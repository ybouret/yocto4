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
                // cleanup
                //______________________________________________________________
                find_limits_of(C);
                show_limits();
                clip_extents();
                
                //______________________________________________________________
                //
                // find dC
                //______________________________________________________________
                mkl::mul_trn(dC, Nu, xi);
                
                
            }
            
            return false;
        }
        
    }
}
