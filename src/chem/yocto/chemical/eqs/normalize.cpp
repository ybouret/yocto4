#include "yocto/chemical/equilibria.hpp"

#include "yocto/math/core/tao.hpp"
#include "yocto/math/core/lu.hpp"

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
        
        void equilibria:: updateScaling()
        {
            for(size_t i=N;i>0;--i)
            {
                GamSF[i] = eqs[i]->compute_scaling(K[i]);
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
            // Load pre-balanced concentrations
            //__________________________________________________________________
            for(size_t j=M;j>0;--j)
            {
                C[j] = C0[j];
                assert(!(active[j]&&C[j]<0));
            }
            
            //__________________________________________________________________
            //
            // Initialize
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
            matrix_t J(N,N);
            
            while(true)
            {
                ++count;
                //______________________________________________________________
                //
                // Gamma And Phi are computed: compute Jacobian W=Phi*Nu'
                //______________________________________________________________
                tao::mmul_rtrn(W,Phi,Nu);
                
                if(! LU<double>::build(W) )
                {
                    std::cerr << "-- Normalize: singular concentrations" << std::endl;
                    return false;
                }
                
                tao::neg(xi,Gamma);
                LU<double>::solve(W, xi);
                clip_extents();
                
                tao::mul_trn(dC, Nu, xi);
                bool converged = true;
                for(size_t j=M;j>0;--j)
                {
                    Cs[j] = C[j];
                    if(active[j])
                    {
                        C[j] += dC[j];
                        if(C[j]<=0)
                        {
                            C[j] = 0;
                        }
                        const double Cj = C[j];
                        const double Dj = dC[j] = fabs(Cj-Cs[j]);
                        if( Dj > Cj * numeric<double>::ftol )
                        {
                            converged = false;
                        }
                    }
                    else
                    {
                        assert(fabs(dC[j])<=0);
                    }
                 
                    
                }

                if(converged)
                {
                    //std::cerr << "// CONVERGED in " << count << " Loops" << std::endl;
                    break;
                }
                updateGammaAndPhi();
               
            }
            
            for(size_t j=M;j>0;--j)
            {
                C0[j] = C[j];
            }
            
            return true;
        }
        
        
    }
    
}

