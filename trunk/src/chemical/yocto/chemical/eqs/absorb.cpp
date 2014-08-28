#include "yocto/chemical/equilibria.hpp"
#include "yocto/math/kernel/algebra.hpp"

#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        typedef math::algebra<double> mkl;
        
        void equilibria:: computeGammaPrimeAndPhi(double t, const array<double> &C)
        {
            iterator     k = begin();
            const size_t n = N;
            for(size_t i=1;i<=n;++i,++k)
            {
                equilibrium &eq = **k;
                xi[i] = eq.computeGammaPrimeAndPhi(Phi[i], t, C, drvs, h);
            }
        }

        void  equilibria:: absorb(double t, array<double> &rho, const array<double> &C)
        {
            assert(C.size()>=M);
            assert(rho.size()>=M);
            
            //__________________________________________________________________
            //
            // compute Phi*rho + dGamma/dt
            //__________________________________________________________________
            computeGammaPrimeAndPhi(t,C); // xi holds dGamma/dt
            mkl::mul_rtrn(W, Phi, Nu);
            if(!lu_t::build(W))
            {
                throw exception("invalid composition for absorbing");
            }
            
            for(size_t i=N;i>0;--i)
            {
                const array<double> &p = Phi[i];
                double sum = 0;
                for(size_t j=M;j>0;--j)
                {
                    sum += p[j] * rho[j];
                }
                xi[i] += sum;
            }
            
            //__________________________________________________________________
            //
            // compute inv(Phi*Nu')*(dtGamma+Phi*rho)
            //__________________________________________________________________
            lu_t::solve(W, xi);
            
            //__________________________________________________________________
            //
            // compute dC = Nu'*inv(Phi*Nu')*(dtGamma+Phi*rho)
            //__________________________________________________________________
            mkl::mul_trn(dC, Nu, xi);
            
            //__________________________________________________________________
            //
            // subtract: all done
            //__________________________________________________________________
            for(size_t j=M;j>0;--j)
            {
                rho[j] -= dC[j];
            }
        }

    }
}
