#include "yocto/chemical/equilibria.hpp"

#include "yocto/math/kernel/tao.hpp"
#include "yocto/math/kernel/det.hpp"
#include "yocto/math/kernel/crout.hpp"
#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"

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
        
        void equilibria:: updateGamma()
        {
            for(size_t i=N;i>0;--i)
            {
                Gamma[i] = eqs[i]->updateGamma(C,K[i]);
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

        void equilibria:: clip_extents()
        {
            for(size_t i=N;i>0;--i)
            {
                equilibrium &eq = *eqs[i];
                eq.compute_limits(C);
                xi[i] = eq.apply_limits(xi[i]);
            }
        }
        
        
        
        bool equilibria:: normalize( array<double> &C0, double t, bool recomputeK )
        {
            
            if(N<=0)
            {
                return true;
            }
            
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
            
            size_t count = 0;
            while(true)
            {
                //______________________________________________________________
                //
                // Starting Point
                //______________________________________________________________
                tao::set(Cs,C);
                std::cerr << std::endl;
                std::cerr << "Gamma=" << Gamma << std::endl;
                std::cerr << "Phi=" << Phi << std::endl;
                tao::mmul_rtrn(W, Phi, Nu);
                std::cerr << "W=" << W << std::endl;
                
                //______________________________________________________________
                //
                // compute full unconstrained Newton Step
                //______________________________________________________________
                if( !crout<double>::build(W) )
                {
                    std::cerr << "-- Normalize: singular concentrations" << std::endl;
                    return false;
                }
                tao::neg(xs,Gamma);
                crout<double>::solve(W,xs);
                
                std::cerr << "xs=" << xs << std::endl;
                
                //______________________________________________________________
                //
                // prepare constraints@starting point
                //______________________________________________________________
                for(size_t i=N;i>0;--i)
                {
                    eqs[i]->compute_limits(Cs);
                }
                
                func_type Fopt( this, &equilibria::H);
                const double    F0 = tao::norm_sq(Gamma);
                const double    F1 = H(1);
                triplet<double> U = { 0,  1,  1 };
                triplet<double> F = { F0, F1, F1};
                
                bracket<double>::expand(Fopt, U, F);
                minimize<double>(Fopt, U, F, 0);
                
                std::cerr << "U=" << U.b << std::endl;
                
                (void)H(U.b);
                for(size_t j=M;j>0;--j)
                {
                    dC[j] = C[j] - Cs[j];
                }
                std::cerr << "dC=" << dC << std::endl;
                
                updateGammaAndPhi();
                if(++count>20)
                    break;
            }
            
            for(size_t j=M;j>0;--j)
            {
                C0[j] = C[j];
            }
            return true;
        }
        

    }

}

