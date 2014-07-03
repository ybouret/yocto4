#include "yocto/chemical/equilibria.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace chemical
    {
        typedef math::algebra<double> mkl;
        
        bool equilibria:: computeNewtonStep( const array<double> &C ) throw()
        {
            
            mkl::mul_rtrn(W, Phi, Nu);
            
            if(LU.build(W))
            {
                mkl::neg(xi, Gamma);
                LU.solve(W,xi);
                
                correct_xi(C);
                
                mkl::mul_trn(dC, Nu, xi);
                return true;
            }
            else
            {
                mkl::set(dC,0.0);
                return false;
            }
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
            if( !computeNewtonStep(C) )
            {
                std::cerr << "-- Normalize: Singular Composition Level 0" << std::endl;
                return false;
            }
            
            size_t count = 0;
        CHECK:
            //__________________________________________________________________
            //
            // Full corrected Newton's step
            //__________________________________________________________________
            std::cerr << "xi=" << xi << std::endl;
            std::cerr << "dC=" << dC << std::endl;
            mkl::add(C,dC);
            const double g1 = updateGammaAndPhi(C);
            std::cerr << "g1=" << g1 << " @" << C << std::endl;
            (void)g1;
            
            if(!computeNewtonStep(C))
            {
                std::cerr << "-- Normalize: Singular Composition Level 1" << std::endl;
                return false;
            }
            if(++count>10) return true;
            goto CHECK;
            
            return true;
        }
        
    }
}
