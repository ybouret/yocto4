#include "yocto/chemical/equilibria.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace chemical
    {
        typedef math::algebra<double> mkl;
        
        //! Warning: Gamma is overwritten !
        double equilibria:: computeTrialFrom(const array<double> &C, const double lambda) throw()
        {
            // build up a trial composition
            for(size_t i=M;i>0;--i)
            {
                Ctmp[i] = C[i] + lambda * dC[i];
            }
            return updateGamma(Ctmp);
        }
        
        
        bool  equilibria:: normalize( double t, array<double> &C )
        {
            
            if(N<=0)
                return true;
            
            // TODO: correct
            
            //__________________________________________________________________
            //
            // First starting point: compute Gamma, Phi, Gradient and constants
            //__________________________________________________________________
            double g0 = computeGammaAndPhi(t,C);
            std::cerr << "g0=" << g0 << " @" << C << std::endl;
            if( !computeNewtonStep() )
            {
                std::cerr << "-- Normalize: Singular Composition Level 0" << std::endl;
                return false;
            }
            
            size_t count = 0;
        CHECK:
            //__________________________________________________________________
            //
            // Moving Full Newton's Step
            //__________________________________________________________________
            for(size_t i=M;i>0;--i)
            {
                Cold[i] = C[i];
                C[i]   += dC[i];
            }
            std::cerr << "C=" << C << std::endl;
            if(!is_valid(C))
            {
                std::cerr << "Should correct !" << std::endl;
                compute_corrector(C);
                std::cerr << "cor=" << dC << std::endl;
                exit(1);
            }
            
            double g1 = updateGammaAndPhi(C);
            (void)g1;
            // blah blah convergence
            if( !computeNewtonStep() )
            {
                std::cerr << "-- Normalize: Singular Composition Level 1" << std::endl;
                return false;
            }
            if( ++count >10 )
                return true;
            goto CHECK;
            
            return true;
        }
        
    }
}