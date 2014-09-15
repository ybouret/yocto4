#include "yocto/math/ztype.hpp"
#include "yocto/math/fit/lsf.hpp"
#include "yocto/math/kernel/crout.hpp"
#include "yocto/math/kernel/tao.hpp"
#include "yocto/code/ipower.hpp"

namespace yocto
{
    namespace math
    {
        
        template <>
        void LeastSquares<real_t>:: Sample:: polynomial(Array             &aorg,
                                                        const array<bool> &used,
                                                        Array             &aerr,
                                                        typename numeric<real_t>::function *transform)
        {
            //__________________________________________________________________
            //
            // prepare fit
            //__________________________________________________________________
            assert(aorg.size() == used.size());
            assert(aorg.size() == aerr.size());
            const size_t n = aorg.size();
            if(n<=0)
            {
                throw exception("Polynomial Fit has not enough coefficients");
            }
            prepare(n);
            size_t dof = N;
            for(size_t i=n;i>0;--i)
            {
                if(used[i])
                {
                    if(dof<=0)
                        throw exception("Polynomial Fit has not enough data");
                    --dof;
                }
            }
            
            //__________________________________________________________________
            //
            // First Pass: compute algebra
            //__________________________________________________________________
            alpha.ldz();
            tao::ld(beta,REAL(0.0));
            tao::ld(aerr,REAL(0.0));
            
            for(size_t i=N;i>0;--i)
            {
                const real_t xi = X[i];
                const real_t yi = (transform != 0) ? (*transform)(Y[i]) : Y[i];
                for(size_t k=n;k>0;--k)
                {
                    const size_t km   = k-1;
                    const real_t xikm = ipower<real_t>(xi,km);
                    beta[k] += yi * xikm;
                    for(size_t l=1;l<=k;++l)
                    {
                        
                        alpha[k][l] += xikm * ipower<real_t>(xi,l-1);
                        alpha[l][k]  = alpha[k][l];
                    }
                }
            }
            
            
            //__________________________________________________________________
            //
            // Second Pass: used variables
            //__________________________________________________________________
            
            // for each row
            for(size_t j=n;j>0;--j)
            {
                if(!used[j])
                {
                    for(size_t k=n;k>0;--k)
                        alpha[j][k] = 0;
                    alpha[j][j] = 1;
                    beta[j]     = aorg[j];
                    continue;
                }
                
                //for each used line
                for(size_t k=n;k>0;--k)
                {
                    if(!used[k])
                    {
                        beta[j]    -= alpha[j][k]*aorg[k];
                        alpha[j][k] = 0;
                    }
                }
            }
            
            //__________________________________________________________________
            //
            // solve
            //__________________________________________________________________
            if( !crout<real_t>::build(alpha) )
            {
                throw exception("Polynomial Fit is Singular");
            }
            tao::set(aorg,beta);
            crout<real_t>::solve(alpha, aorg);
            
            
            //__________________________________________________________________
            //
            // compute errors by covariance
            //__________________________________________________________________
            D = 0;
            for(size_t i=N;i>0;--i)
            {
                const real_t xi = X[i];
                const real_t yi = transform ? (*transform)(Y[i]) : Y[i];
                Z[i] = 0;
                for(size_t j=1;j<=n;++j)
                {
                    Z[i] += aorg[j] * ipower(xi,j-1);
                }
                const real_t dd = yi - Z[i];
                D += dd*dd;
            }
            
            matrix<real_t> curv(n,n);
            curv.ld1();
            crout<real_t>::solve(alpha,curv);
            
            const real_t residue=D/(dof+1);
            for(size_t k=n;k>0;--k)
            {
                if(used[k])
                {
                    aerr[k] = Sqrt( residue * Fabs(curv[k][k]) );
                }
            }
            
        }
        
        
    }
    
}

