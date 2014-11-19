#include "yocto/chemical/equilibria.hpp"

#include "yocto/math/kernel/tao.hpp"
#include "yocto/math/kernel/det.hpp"
#include "yocto/math/kernel/crout.hpp"
//#include "yocto/math/opt/bracket.hpp"
//#include "yocto/math/opt/minimize.hpp"
#include "yocto/sort/index.hpp"

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
#if 0
            //__________________________________________________________________
            //
            // initializing constants
            //__________________________________________________________________
            if(recomputeK)
            {
                computeK(t);
            }
            
            //__________________________________________________________________
            //
            // indexing
            //__________________________________________________________________
            uvector_t Kid(N,0);
            make_index(K,Kid,__compare<size_t> );
            std::cerr << "K="   << K   << std::endl;
            std::cerr << "Kid=" << Kid << std::endl;
            for(size_t i=N;i>0;--i)
            {
                adjust( *eqs[ Kid[i] ] );
                //exit(1);
            }
#endif
            
            
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
                // Gamma And Phi are computed: compute Jacobian W=Phi*Nu'
                //______________________________________________________________
                tao::mmul_rtrn(W,Phi,Nu);
                std::cerr << std::endl;
                std::cerr << "Gamma=" << Gamma << std::endl;
                std::cerr << "Phi="   << Phi   << std::endl;
                std::cerr << "W=" << W << std::endl;
                
                
                //______________________________________________________________
                //
                // Compute full Newton's step = -inv(W)*Gamma
                //______________________________________________________________
                if( !crout<double>::build(W) )
                {
                    std::cerr << "-- Normalize: singular system" << std::endl;
                    return false;
                }
                
                tao::neg(xi,Gamma);
                crout<double>::solve(W,xi);
                std::cerr << "xi0=" << xi << std::endl;
                clip_extents();
                std::cerr << "xi="  << xi << std::endl;
                
                //______________________________________________________________
                //
                // estimate dC
                //______________________________________________________________
                tao::mul_trn(dC, Nu, xi);
                std::cerr << "dC=" << dC << std::endl;
                for(size_t j=M;j>0;--j)
                {
                    Cs[j] = C[j];
                    if(active[j])
                    {
                        C[j] += dC[j];
                        if(C[j]<=0) C[j] = 0;
                    }
                    else
                    {
                        assert(fabs(dC[j])<=0);
                    }
                    dC[j] = fabs(C[j] - Cs[j]);
                }
                std::cerr << "C=" << C << std::endl;
                std::cerr << "err=" << dC << std::endl;
                if(++count>20)
                    break;
                
                updateGammaAndPhi();
            }
            
            
            return false;
        }
        
        
    }
    
}

