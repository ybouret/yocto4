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
        
        double equilibria:: GammaNorm()
        {
            double ans = 0;
            for(size_t i=N;i>0;--i)
            {
                const equilibrium &eq = *eqs[i];
                double g = fabs(Gamma[i]);
                if(eq.SumNuP>0)
                {
                    g = pow(g,1.0/eq.SumNuP);
                }
                ans += g;
            }
            return ans/N;
        }

        
        double equilibria:: H(double U)
        {
            for(size_t j=M;j>0;--j)
            {
                C[j] = Cs[j] + U * dC[j];
                if(active[j])
                {
                    if(C[j]<=0) C[j] = 0;
                }
            }
            updateGamma();
            return GammaNorm();
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
            
            //computeGammaScaling();
            //std::cerr << "Scaling=" << GamSF << std::endl;
            
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
                tao::neg(xi,Gamma);
                crout<double>::solve(W,xi);
                
                std::cerr << "xi0=" << xi << std::endl;
                
                //______________________________________________________________
                //
                // clip constraints
                //______________________________________________________________
                for(size_t i=N;i>0;--i)
                {
                    eqs[i]->compute_limits(Cs);
                    xi[i] = eqs[i]->apply_limits(xi[i]);
                }
                std::cerr << "xi1=" << xi << std::endl;

                //______________________________________________________________
                //
                // initial dC
                //______________________________________________________________
                tao::mul_trn(dC, Nu, xi);
                std::cerr << "dC=" << dC << std::endl;
                
                const double F0 = GammaNorm();
                
                func_type    Fn(this,&equilibria::H);
                const double F1 = Fn(1);
                
                std::cerr << "F: " << F0 << " --> " << F1 << std::endl;
                
                if( F1>= F0 )
                {
                    std::cerr << "Need To Backtrack" << std::endl;
                    triplet<double> U = {0, 1, 1 };
                    triplet<double> F = {F0,F1,F1};
                    if(!bracket<double>::inside(Fn,U,F))
                    {
                        std::cerr << "-- Bracketing Failure..." << std::endl;
                        return false;
                    }
                    minimize<double>(Fn, U, F, 0);
                    std::cerr << "U=" << U.b << std::endl;
                    Fn(U.b);
                }
                
                // compute effective dC
                for(size_t j=M;j>0;--j)
                {
                    dC[j] = C[j] - Cs[j];
                }
                std::cerr << "delta=" << dC << std::endl;
                
                updateGammaAndPhi();
                if(++count>10)
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

