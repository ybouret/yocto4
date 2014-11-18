#include "yocto/chemical/equilibria.hpp"

#include "yocto/math/kernel/tao.hpp"
#include "yocto/math/kernel/det.hpp"
#include "yocto/math/kernel/crout.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        using namespace math;

        
        void equilibria:: updateGammaAndPhi()
        {
            for(size_t i=N;i>0;--i)
            {
                Gamma[i] = eqs[i]->updateGammaAndPhi(C,K[i],Phi[i]);
            }
        }
        
        void equilibria:: computeGammaAndPhi( double t)
        {
            for(size_t i=N;i>0;--i)
            {
                Gamma[i] = eqs[i]->computeGammaAndPhi(t,C, K[i], Phi[i]);
            }
        }
    
        
        void equilibria:: computeK(double t)
        {
            for(size_t i=N;i>0;--i)
            {
                K[i] = eqs[i]->K(t);
            }
        }

        
        bool equilibria:: normalize( array<double> &C0, double t, bool recomputeK )
        {
            
            //__________________________________________________________________
            //
            // balancing
            //__________________________________________________________________
            if(!balance(C0))
            {
                return false;
            }
            
            std::cerr << "C=" << C << std::endl;
            //__________________________________________________________________
            //
            // initializing: at this point, C=C0
            //__________________________________________________________________
            if(recomputeK)
            {
                computeGammaAndPhi(t);
            }
            else
            {
                updateGammaAndPhi();
            }
            
            while(true)
            {
                std::cerr << "Gamma=" << Gamma << std::endl;
                std::cerr << "Phi=" << Phi << std::endl;
                tao::mmul_rtrn(W, Phi, Nu);
                std::cerr << "W=" << W << std::endl;
                
             
                break;
            }
            
            
            return true;
        }
        

    }

}

