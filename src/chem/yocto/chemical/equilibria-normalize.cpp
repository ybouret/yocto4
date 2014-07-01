#include "yocto/chemical/equilibria.hpp"

namespace yocto
{
    namespace chemical
    {
        //! Warning: Gamma is overwritten !
        double equilibria:: computeTrialFrom(const array<double> &C, const double lambda) throw()
        {
            for(size_t i=M;i>0;--i)
            {
                Ctry[i] = C[i] + lambda * dC[i];
            }
            return updateGamma(Ctry);
        }

        
        bool  equilibria:: normalize( double t, array<double> &C )
        {
            
            //__________________________________________________________________
            //
            // First starting point: compute Gamma, Phi, Gradient and constants
            //__________________________________________________________________
            double F0 = computeGammaAndPhi(t,C);
            std::cerr << "F0=" << F0 << " @" << C << std::endl;
            if( !computeNewtonStep() )
            {
                std::cerr << "-- Normalize: Singular Composition Level 0" << std::endl;
            }
            
            //__________________________________________________________________
            //
            //__________________________________________________________________
            const double F1 = computeTrialFrom(C, 1.0);
            std::cerr << "F1=" << F1 << " @" << Ctry << std::endl;
            
            return true;
        }

    }
}