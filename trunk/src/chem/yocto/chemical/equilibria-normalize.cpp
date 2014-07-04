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
            
            if(N<=0)
                return true;
            
            // TODO: validate concentrations
            validate(C);
            
            exit(1);
            
            //__________________________________________________________________
            //
            // First starting point: compute Gamma, Phi constants
            //__________________________________________________________________
            double g0 = computeGammaAndPhi(t,C);
            std::cerr << "K=" << K << std::endl;
            std::cerr << "@" << C << "; g0=" << g0 <<std::endl;
            std::cerr << "Gamma=" << Gamma << std::endl;
            
            size_t count = 0;
        CHECK:
            //__________________________________________________________________
            //
            // Prepare Newton's setp
            //__________________________________________________________________
            mkl::mul_rtrn(W, Phi, Nu);
            if(!LU.build(W))
            {
                std::cerr << "-- Normalize: singular solution..." << std::endl;
            }
            
            //__________________________________________________________________
            //
            // compute extent
            //__________________________________________________________________
            mkl::neg(xi, Gamma);
            LU.solve(W,xi);
            
            //__________________________________________________________________
            //
            // correct extent
            //__________________________________________________________________
            limits_of(C);
            correct_xi();
            
            //__________________________________________________________________
            //
            // Deduce corrected Newton's step
            //__________________________________________________________________
            mkl::mul_trn(dC, Nu, xi);
            
            //__________________________________________________________________
            //
            // Move
            //__________________________________________________________________
            mkl::add(C,dC);
            const double g1 = updateGamma(C);
            std::cerr << "@" << C << "; g1=" << g1 <<std::endl;
            std::cerr << "Gamma=" << Gamma << std::endl;
            if(++count>3)
            {
                return true;
            }
            updatePhi(C);
            goto CHECK;
            
            return true;
        }
        
    }
}
