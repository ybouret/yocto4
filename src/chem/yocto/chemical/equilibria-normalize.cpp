#include "yocto/chemical/equilibria.hpp"
#include "yocto/math/kernel/algebra.hpp"

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
                Ctry[i] = C[i] + lambda * dC[i];
            }
            return updateGamma(Ctry);
        }
        
        
        bool  equilibria:: normalize( double t, array<double> &C )
        {
            static const double ALPHA = 1e-4;
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
            // Loop On Newton's Step
            //__________________________________________________________________
            size_t count = 0;
            while(true)
            {
                //______________________________________________________________
                //
                // compute descent rate
                //______________________________________________________________
                const double rate = mkl::dot(grad,dC);
                std::cerr << "rate=" << rate << std::endl;
                if(rate>=0)
                {
                    std::cerr << "-- Normalize: descente rate round off reached !" << std::endl;
                    goto CONVERGED;
                }
                
                //______________________________________________________________
                //
                // Move to full Newton's step
                //______________________________________________________________
                for(size_t i=M;i>0;--i)
                {
                    Ctry[i] = C[i] + dC[i];
                }
                const double F1 = updateGamma(Ctry);
                std::cerr << "F1=" << F1 << " @" << Ctry << std::endl;
                std::cerr << "Gamma=" << Gamma << std::endl;
                if( F1 <= F0 + rate * ALPHA )
                {
                    std::cerr << "Accept Newton's Step" << std::endl;
                    mkl::set(C,Ctry);
                    //__________________________________________________________
                    //
                    // Test convergence
                    //__________________________________________________________
                    
                    //__________________________________________________________
                    //
                    // Compute Next Newton's step:
                    // Gamma is already up to date, only need to recompute Phi
                    // and copy the F value
                    //__________________________________________________________
                    updatePhi(C);
                    if(!computeNewtonStep())
                    {
                        std::cerr << "-- Normalize: Singular Composition Level-1" << std::endl;
                        return false;
                    }
                    F0 = F1;
                    if(++count>10)
                        break;
                }
                else
                {
                    std::cerr << "Need Backtracking" << std::endl;
                    exit(1);
                }
                
            }
            
        CONVERGED:
            return true;
        }
        
    }
}