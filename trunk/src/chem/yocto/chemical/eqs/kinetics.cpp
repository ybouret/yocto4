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


        void equilibria:: absorb(const double         t,
                                 array<double>       &rho,
                                 const array<double> &C0)
        {
            
            assert(C0.size()>=M);
            assert(rho.size()==C0.size());
            tao::set(C,C0);
            tao::set(dC,rho);
            
            // check validity
            computeGammaAndPhi(t);
            tao::mmul_rtrn(W, Phi, Nu);
            if(!crout<double>::build(W))
            {
                throw exception("equilibria.absorb(invalid concentrations)");
            }
            
            // build rhs vector in xi
            tao::mul(xi,Phi,dC);
            for(size_t i=N;i>0;--i)
            {
                equilibrium &eq = *eqs[i];
                //std::cerr << eq.name << ": reac_weight=" << eq.reac_weight << std::endl;
                dtGam[i] = drvs( eq.K, t, dt_drvs) * eq.reac_weight;
                xi[i]   += dtGam[i];
            }
            //std::cerr << "dtGam=" << dtGam << std::endl;
            //std::cerr << "rhs=" << xi << std::endl;
            //std::cerr << "Gamma=" << Gamma << std::endl;
            //std::cerr << "Phi="   << Phi   << std::endl;
            //std::cerr << "Nu="    << Nu    << std::endl;
            
            // find -xi
            crout<double>::solve(W,xi);
            
            // compute delta rho
            tao::mul_trn(dC, Nu, xi);
            //std::cerr << "drho=" << dC << std::endl;
            for(size_t j=M;j>0;--j)
            {
                rho[j] -= dC[j];
            }
        }

        
        
    }

}

