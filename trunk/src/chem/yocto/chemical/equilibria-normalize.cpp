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
                Ctry[i] = C[i] + lambda * dC[i];
            }
            return updateGamma(Ctry);
        }
        
        
        bool  equilibria:: normalize( double t, array<double> &C )
        {
            static const double ALPHA         = 1e-4;
            static const double BACKTRACK_MIN = 0.1;
            
            if(N<=0)
                return true;
            
            //__________________________________________________________________
            //
            // First starting point: compute Gamma, Phi, Gradient and constants
            //__________________________________________________________________
            double g0 = computeGammaAndPhi(t,C);
            std::cerr << "g0=" << g0 << " @" << C << std::endl;
            if( !computeNewtonStep() )
            {
                std::cerr << "-- Normalize: Singular Composition Level 0" << std::endl;
            }
            
            //__________________________________________________________________
            //
            // Loop On Newton's Step
            //__________________________________________________________________
            size_t count = 0;
        CHECK_STEP:
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
                double g1 = updateGamma(Ctry);
                std::cerr << "F1=" << g1 << " @" << Ctry << std::endl;
                std::cerr << "Gamma=" << Gamma << std::endl;
                if( g1 <= g0 + rate * ALPHA )
                {
                    std::cerr << "Accept Newton's Step" << std::endl;
                    mkl::set(C,Ctry);
                    //__________________________________________________________
                    //
                    // Test convergence
                    //__________________________________________________________
                    
                    // blah blah
                    
                    if(++count>10)
                    {
                        std::cerr << "COUNT MAX" << std::endl;
                        goto CONVERGED;
                    }
                }
                else
                {
                    //__________________________________________________________
                    //
                    // We want to backtrack until g1 <= g0 - (-rate * ALPHA) * lam
                    // But we allow to decrease only down to BACKTRACK_MIN
                    //__________________________________________________________
                    
                    std::cerr << "Need Backtracking" << std::endl;
                    const double slope = -rate * ALPHA; assert(slope>0);
                    double lam = 1;
                    do
                    {
                        lam /= 2;
                        g1   = computeTrialFrom(C, lam);
                        std::cerr << "g(" << lam << ")=" << g1 << " @" << Ctry << std::endl;
                    } while( (g1>g0-slope*lam) && lam>BACKTRACK_MIN);
                    
                    std::cerr << "lam=" << lam << std::endl;
                    mkl::set(C,Ctry);
                }
                
                //__________________________________________________________
                //
                // Compute Next Newton's step: at this point,
                // Gamma is already up to date, only need to recompute Phi
                // and copy the g value
                //__________________________________________________________
                updatePhi(C);
                if(!computeNewtonStep())
                {
                    std::cerr << "-- Normalize: Singular Composition Level-1" << std::endl;
                    return false;
                }
                g0 = g1;
                
                
                goto CHECK_STEP;
            }
            
        CONVERGED:
            return true;
        }
        
    }
}