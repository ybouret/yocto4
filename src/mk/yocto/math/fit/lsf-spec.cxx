#include "yocto/math/ztype.hpp"
#include "yocto/math/fit/lsf.hpp"
#include "yocto/math/core/lu.hpp"
#include "yocto/math/core/tao.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace math
    {
        
        template <>
        void LeastSquares<real_t>:: Sample:: polynomial(Array             &aorg,
                                                        const array<bool> &used,
                                                        Array             &aerr,
                                                        numeric<real_t>::function *transform)
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
            if( !LU<real_t>::build(alpha) )
            {
                throw exception("Polynomial Fit is Singular");
            }
            tao::set(aorg,beta);
            LU<real_t>::solve(alpha, aorg);
            
            
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
            LU<real_t>::solve(alpha,curv);
            
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


namespace yocto
{
    namespace math
    {
        template <>
        void LeastSquares<real_t>::Sample:: Pade(Array &            P,
                                                 const array<bool> &usedP,
                                                 Array &            Q,
                                                 const array<bool>  &usedQ)
        {
            assert( P.size() == usedP.size() );
            assert( Q.size() == usedQ.size() );
            const size_t p   = P.size(); //!< denominator
            const size_t q   = Q.size(); //!< numerator
            const size_t dim = p+q;      //!< total dimension
            if(dim<=0)
                throw exception("not enough coefficients for Pade fit");
            
            prepare(dim);
            array<real_t>  &aorg = dFdu; assert(aorg.size()==dim);
            vector<bool>    used(dim,true);
            
            size_t dof = N;
            for(size_t i=1;i<=p;++i)
            {
                if(usedP[i])
                {
                    if(dof<=0)
                        throw exception("Pade: not enough data");
                    --dof;
                }
                aorg[i] = P[i];
                used[i] = usedP[i];
            }
            
            for(size_t i=1;i<=q;++i)
            {
                if(usedQ[i])
                {
                    if(dof<=0)
                        throw exception("Pade: not enough data");
                    --dof;
                }
                const size_t j = p+i;
                aorg[j] = Q[i];
                used[j] = usedQ[i];
            }
            
            //__________________________________________________________________
            //
            // Right Hand Side
            //__________________________________________________________________
            for(size_t r=1;r<=p;++r)
            {
                real_t       s  = 0;
                const size_t pw = r-1;
                for(size_t i=1;i<=N;++i)
                {
                    s += Y[i] * ipower(X[i], pw);
                }
                beta[r] = s;
            }
            
            for(size_t r=1;r<=q;++r)
            {
                real_t s  = 0;
                for(size_t i=1;i<=N;++i)
                {
                    const real_t yi = Y[i];
                    s += (yi*yi) * ipower(X[i],r);
                }
                beta[r+p] = s;
            }
            
            //__________________________________________________________________
            //
            // top left quadrant
            //__________________________________________________________________
            for(size_t r=1;r<=p;++r)
            {
                for(size_t c=r;c<=p;++c)
                {
                    real_t       s  = 0;
                    const size_t pw = r+c-2;
                    for(size_t i=1;i<=N;++i)
                    {
                        const real_t xi = X[i];
                        s += ipower(xi,pw);
                    }
                    alpha[r][c] = alpha[c][r] = s;
                }
            }
            
            //__________________________________________________________________
            //
            // bottom right quadrant
            //__________________________________________________________________
            for(size_t r=1;r<=q;++r)
            {
                const size_t ir = r+p;
                for(size_t c=r;c<=q;++c)
                {
                    const size_t ic = c+p;
                    real_t       s  = 0;
                    const size_t pw = c+r;
                    for(size_t i=1;i<=N;++i)
                    {
                        const real_t yi = Y[i];
                        s += (yi*yi) * ipower(X[i],pw);
                    }
                    alpha[ir][ic] = alpha[ic][ir] = -s;
                }
            }
            
            //__________________________________________________________________
            //
            // extra diagonal
            //__________________________________________________________________
            for(size_t r=1;r<=p;++r)
            {
                for(size_t c=1;c<=q;++c)
                {
                    const size_t ic = c+p;
                    real_t       s  = 0;
                    const size_t pw = r+c-1;
                    for(size_t i=1;i<=N;++i)
                    {
                        s += Y[i] * ipower( X[i], pw);
                    }
                    alpha[r][ic] = -(alpha[ic][r] = s );
                }
            }
            
            //__________________________________________________________________
            //
            // Regularizing
            //__________________________________________________________________
            // for each row
            for(size_t j=dim;j>0;--j)
            {
                if(!used[j])
                {
                    for(size_t k=dim;k>0;--k)
                    {
                        alpha[j][k] = 0;
                    }
                    alpha[j][j] = 1;
                    beta[j]     = aorg[j];
                    continue;
                }
                
                //for each used line
                for(size_t k=dim;k>0;--k)
                {
                    if(!used[k])
                    {
                        beta[j]    -= alpha[j][k]*aorg[k];
                        alpha[j][k] = 0;
                    }
                }
            }
            
            
            
            //std::cerr << "alpha=" << alpha << std::endl;
            
            
            //__________________________________________________________________
            //
            // solve the system
            //__________________________________________________________________
            if( !LU<real_t>::build(alpha) )
                throw exception("singular Pade approximant");
            LU<real_t>::solve(alpha, beta);
            
            //__________________________________________________________________
            //
            // dispatch coefficients
            //__________________________________________________________________
            for(size_t i=1;i<=p;++i) P[i] = beta[i];
            for(size_t i=1;i<=q;++i) Q[i] = beta[p+i];
            
            //__________________________________________________________________
            //
            // compute the fit values
            //__________________________________________________________________
            
            for(size_t i=1;i<=N;++i)
            {
                const real_t xi = X[i];
                real_t num = REAL(0.0);
                for(size_t j=1;j<=p;++j) num += P[j]*ipower(xi,j-1);
                real_t den = REAL(1.0);
                for(size_t j=1;j<=q;++j) den += Q[j]*ipower(xi,j);
                Z[i] = num/den;
            }
            
        }
    }
    
}

