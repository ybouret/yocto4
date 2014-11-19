#include "yocto/chemical/equilibria.hpp"

#include "yocto/math/kernel/tao.hpp"
#include "yocto/math/kernel/det.hpp"
#include "yocto/math/kernel/crout.hpp"
#include "yocto/sort/index.hpp"
#include "yocto/code/ipower.hpp"

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
#if 0
        void equilibria:: adjust( equilibrium &eq )
        {
            const size_t   i   = eq.indx;
            array<double> &phi = Phi[i];
            const double   Ki  = K[i];
            vector<double> nu(M,0);
            for(size_t j=M;j>0;--j) nu[j] = Nu[i][j];
            std::cerr << "-------- [[ " << eq.name << " ]] --------" << std::endl;
            std::cerr << "C" << i << "=" << C << std::endl;
            std::cerr << "nu=" << nu << std::endl;
            
            size_t count = 0;
            while(true)
            {
                const double G = eq.updateGammaAndPhi(C, Ki, phi);
                std::cerr << "\tG=" << G << std::endl;
                std::cerr << "\tphi=" << phi << std::endl;
                const double sig = tao::dot(phi, nu);
                std::cerr << "\tsig=" << sig << std::endl;
                if(sig>=0)
                {
                    return;
                }
                const double xnewt = G/(-sig);
                std::cerr << "\txnewt=" << xnewt << std::endl;
                eq.compute_limits(C);
                eq.show_limits(std::cerr);
                const double x = eq.apply_limits(xnewt);
                std::cerr << "\tx=" << x << std::endl;
                if(fabs(x)<=0)
                    return;
                for(size_t j=M;j>0;--j)
                {
                    if(active[j])
                    {
                        C[j] += nu[j] * x;
                        if(C[j]<=0) C[j] = 0;
                    }
                }
                std::cerr << "C" << i << "=" << C << std::endl;
                if(++count>20)
                    break;
            }
            
        }
#endif
        
        double equilibria:: H(double omega)
        {
            const array<double> &oldC = Cs;
            const array<double> &newC = dC;
            const double omw = 1.0 - omega;
            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    C[j] = omw * oldC[j] + omega * newC[j];
                }
                else
                {
                    C[j] = oldC[j];
                }
            }
            updateGamma();
            return tao::norm(Gamma);
        }
        
        
        
        bool equilibria:: normalize( array<double> &C0, double t, bool recomputeK )
        {
            
            if(N<=0)
            {
                return true;
            }
            
            //__________________________________________________________________
            //
            // Load concentrations
            //__________________________________________________________________
            for(size_t j=M;j>0;--j)
            {
                C[j] = C0[j];
                assert(!(active[j]&&C[j]<0));
            }
            
            
            std::cerr << "C=" << C << std::endl;
            
            if(recomputeK)
            {
                computeGammaAndPhi(t);
            }
            else
            {
                updateGammaAndPhi();
            }
            
            typedef crout<double> LU;
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
                
                //std::cerr << std::endl;
                //std::cerr << "// Loop #" << count << std::endl;
                //std::cerr << "Gamma=" << Gamma << std::endl;
                
                if(! LU::build(W) )
                {
                    std::cerr << "-- Normalize: singular concs..." << std::endl;
                    return false;
                }
                
                tao::neg(xi,Gamma);
                LU::solve(W, xi);
                //std::cerr << "xi0=" << xi << std::endl;
                clip_extents();
                //std::cerr << "xi="  << xi << std::endl;
                
                tao::mul_trn(dC, Nu, xi);
                //std::cerr << "dC=" << dC << std::endl;
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
                //std::cerr << "dC=" << dC << std::endl;
                if(converged)
                {
                    std::cerr << "// CONVERGED in " << count << " Loops" << std::endl;
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

