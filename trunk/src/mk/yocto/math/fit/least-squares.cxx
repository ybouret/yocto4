#include "yocto/math/ztype.hpp"
#include "yocto/math/fit/least-squares.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/ipower.hpp"

#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"



namespace yocto
{
    namespace math
    {
        
        typedef algebra<real_t> mkl;
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // sample operations
        //
        ////////////////////////////////////////////////////////////////////////
        
        template <>
        least_squares<real_t>:: sample:: ~sample() throw()
        {
        }
        
        template <>
        least_squares<real_t>:: sample:: sample(const array<real_t> &aX, const array<real_t> &aY, array<real_t> &aZ ) throw() :
        X(aX),
        Y(aY),
        Z(aZ),
        N(0),
        beta(),
        alpha(),
        Gamma(),
        u(),
        dFdu(),
        dFda(),
        D2(0)
        {
            
        }
        
        template <>
        void least_squares<real_t>:: sample::release() throw()
        {
            dFda.  release();
            dFdu.  release();
            u.     release();
            Gamma. release();
            alpha. release();
            beta.  release();
            D2 = 0;
        }
        
        
        template <>
        void least_squares<real_t>:: sample:: prepare(size_t nvar, size_t npar)
        {
            assert(nvar>0);
            assert(npar>0);
            assert(X.size()==Y.size());
            assert(X.size()==Z.size());
            release();
            
            (size_t &)N = X.size();
            
            beta.  make(nvar,0);
            alpha. make(nvar,nvar);
            Gamma. make(npar,nvar);
            u.     make(npar,0);
            dFdu.  make(npar,0);
            dFda.  make(nvar,0);
            
            if(nvar==npar) Gamma.ld1();
        }
        template <>
        void least_squares<real_t>:: sample:: set_parameter(size_t ipar, size_t ivar) throw()
        {
            assert(ipar>0);
            assert(ivar>0);
            assert(ipar<=Gamma.rows);
            assert(ivar<=Gamma.cols);
            Gamma[ipar][ivar] = 1;
        }
        
        
        template <>
        real_t least_squares<real_t>:: sample:: compute_D2( function &F, const array<real_t> &a )
        {
            mkl::mul(u, Gamma, a);
            D2  = 0;
            for(size_t j=N;j>0;--j)
            {
                Z[j] = F(X[j],u);
                const real_t dd = Z[j] - Y[j];
                D2 += dd*dd;
            }
            return D2;
        }
        
        namespace {
            
            class partialF
            {
            public:
                least_squares<real_t>::function &F;
                array<real_t>                   &U;
                real_t                           x;
                size_t                           i;
                
                inline partialF( least_squares<real_t>::function &aF, array<real_t> &aU) throw() :
                F(aF), U(aU), x(0), i(0)
                {
                }
                
                inline ~partialF() throw()
                {
                    
                }
                
                inline double compute( real_t u )
                {
                    assert(i>0);
                    assert(i<=U.size());
                    
                    const real_t U_save = U[i];
                    U[i]                = u;
                    const real_t ans    = F(x,U);
                    U[i] = U_save;
                    return ans;
                }
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(partialF);
                
                
            };
        }
        
        template <>
        real_t least_squares<real_t>:: sample:: compute_D2(function            &F,
                                                           const array<real_t> &a,
                                                           const array<bool>   &used,
                                                           derivative<real_t>  &drvs,
                                                           const real_t         h)
        {
            partialF                  pF( F, u );
            numeric<real_t>::function FU( &pF, & partialF::compute);
            const size_t              n = dFdu.size();
            const size_t              m = dFda.size();
            assert(alpha.cols==m);
            
            //__________________________________________________________________
            //
            // compute parameters and set D2, beta and alpha to 0
            //__________________________________________________________________
            mkl::mul(u, Gamma, a);
            D2  = 0;
            mkl::set(beta,0);
            alpha.ldz();
            
            //__________________________________________________________________
            //
            // build D2, beta and alpha
            //__________________________________________________________________
            for(size_t j=N;j>0;--j)
            {
                
                Z[j] = F( (pF.x = X[j]),u);
                const real_t dd = Y[j]-Z[j];
                D2 += dd*dd;
                
                // compute reduced gradient
                for(size_t k=n;k>0;--k)
                {
                    pF.i = k;
                    dFdu[k] = drvs( FU, u[k], h );
                }
                
                // deduce full gradient
                mkl::mul_trn(dFda, Gamma, dFdu);
                
                for(size_t k=m;k>0;--k)
                {
                    if(used[k])
                    {
                        const real_t g = dFda[k];
                        beta[k] += dd * g;
                        for(size_t l=1;l<=k;++l)
                        {
                            if(used[l])
                            {
                                alpha[k][l] += g * dFda[l];
                                alpha[l][k]  = alpha[k][l];
                            }
                        }
                    }
                }
                
            }
            
            //__________________________________________________________________
            //
            // The beta components for unused variables
            // and the row/colums of alpha for unused variables are zero !!!
            //__________________________________________________________________
            
            
            return D2;
        }
        
        
        
        template <>
        void least_squares<real_t>:: sample:: polynomial(array<real_t>             &aorg,
                                                         const array<bool>         &used,
                                                         array<real_t>             &aerr,
                                                         numeric<real_t>::function *transform)
        {
            
            //__________________________________________________________________
            //
            // prepare fit
            //__________________________________________________________________
            assert(aorg.size() == used.size());
            assert(aorg.size() == aerr.size());
            const size_t n = aorg.size();
            if(n<=0) throw exception("not enough coefficients for polynomial fit");
            prepare(n);
            crout<real_t> LU(n);
            size_t dof = N;
            for(size_t i=n;i>0;--i)
            {
                if(used[i])
                {
                    if(dof<=0)
                        throw exception("lsf/polynomial: not enough data");
                    --dof;
                }
            }
            
            //__________________________________________________________________
            //
            // First Pass: compute algebra
            //__________________________________________________________________
            alpha.ldz();
            mkl::set(beta,REAL(0.0));
            mkl::set(aerr,REAL(0.0));
            
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
            
            if( !LU.build(alpha) )
            {
                throw exception("lsf/polynomial: singular fit");
            }
            mkl::set(aorg,beta);
            crout<real_t>::solve(alpha, aorg);
            
            
            //__________________________________________________________________
            //
            // compute errors by covariance
            //__________________________________________________________________
            D2 = 0;
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
                D2 += dd*dd;
            }
            
            matrix<real_t> curv(n,n);
            curv.ld1();
            crout<real_t>::solve(alpha,curv);
            if(dof>0)
            {
                const real_t residue=D2/dof;
                for(size_t k=1;k<=n;++k)
                {
                    if(used[k])
                    {
                        aerr[k] = Sqrt( residue * Fabs(curv[k][k]) );
                    }
                }
            }
            
        }
        
        ////////////////////////////////////////////////////////////////////////
        //
        // Pade
        //
        ////////////////////////////////////////////////////////////////////////
        template <>
        void least_squares<real_t>:: sample:: Pade(array<real_t>     &P,
                                                   const array<bool> &usedP,
                                                   array<real_t>     &Q,
                                                   const array<bool> &usedQ)
        {
            assert( P.size() == usedP.size() );
            assert( Q.size() == usedQ.size() );
            const size_t p   = P.size(); //!< denominator
            const size_t q   = Q.size(); //!< numerator
            const size_t dim = p+q;      //!< total dimension
            if(dim<=0)
                throw exception("not enough coefficients for Pade");
            
            prepare(dim);
            array<real_t>  &aorg = dFda; assert(aorg.size()==dim);
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
                    const real_t ic = c+p;
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
                    const real_t ic = c+p;
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
                        alpha[j][k] = 0;
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
            crout<real_t> lu(dim);
            if( !lu.build(alpha) )
                throw exception("singular Pade approximant");
            crout<real_t>::solve(alpha, beta);
            
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
        
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // samples operations
        //
        ////////////////////////////////////////////////////////////////////////
        
        template <>
        least_squares<real_t>:: samples:: samples() throw() :
        vector<sample::pointer>()
        {
        }
        
        template <>
        least_squares<real_t>:: samples:: ~samples() throw() {}
        
        template <>
        void least_squares<real_t>:: samples:: append(const array<real_t> &aX, const array<real_t> &aY, array<real_t> &aZ)
        {
            const sample::pointer S( new sample(aX,aY,aZ) );
            push_back(S);
        }
        
        
        ////////////////////////////////////////////////////////////////////
        //
        // least squares
        //
        ////////////////////////////////////////////////////////////////////
        template <>
        least_squares<real_t>:: ~least_squares() throw()
        {
        }
        
        template <>
        real_t least_squares<real_t>::compute_D2_org()
        {
            real_t ans = 0;
            
            alpha.ldz();
            mkl::set(beta,0);
            for(size_t i=1;i<=nums;++i)
            {
                sample &S = *(*data)[i];
                ans += S.compute_D2(*proc, aorg, used, drvs, h);
                
                for(size_t k=nvar;k>0;--k)
                {
                    beta[k] += S.beta[k];
                    for(size_t l=nvar;l>0;--l)
                    {
                        alpha[k][l] += S.alpha[k][l];
                    }
                }
            }
            
            
            for(size_t k=nvar;k>0;--k)
            {
                if( ! used[k] ) alpha[k][k] = REAL(1.0);
            }
            
            return ans;
        }
        
        
        template <>
        least_squares<real_t>:: least_squares() :
        D2(0),
        drvs(),
        h(REAL(1.0e-4)),
        ftol(numeric<real_t>::ftol),
        verbose(false),
        proc(0),
        data(0),
        nvar(0),
        nums(0),
        alpha(),
        curv(),
        beta(),
        step(),
        aorg(),
        used(),
        atmp(),
        LU()
        {
            
        }
        
        template <>
        void least_squares<real_t>:: cleanup() throw()
        {
            LU.    release();
            used.  release();
            aorg.  release();
            step.  release();
            beta.  release();
            alpha. release();
        }
        
        template <>
        const char least_squares<real_t>::fn[]   = "[least_squares] ";
        
        
        template <>
        bool least_squares<real_t>:: build_curvature(real_t lambda)
        {
            if(verbose) std::cerr << fn << "lambda = " << lambda << std::endl;
            const real_t fac  = REAL(1.0) + lambda;
            for(size_t k=nvar;k>0;--k)
            {
                for(size_t l=nvar;l>0;--l)
                {
                    const real_t aa = alpha[k][l];
                    curv[k][l] = (k == l) ? fac * aa : aa;
                }
            }
            
            return LU.build(curv);
        }
        
        template <>
        real_t least_squares<real_t>:: compute_D2_tmp( real_t z )
        {
            
            for(size_t k=nvar;k>0;--k)
            {
                atmp[k] = aorg[k] + z*step[k];
            }
            
            real_t ans = 0;
            for(size_t i=1;i<=nums;++i)
            {
                sample &S = *(*data)[i];
                ans += S.compute_D2(*proc, atmp);
            }
            
            return ans;
        }
        
        template <>
        least_squares_result least_squares<real_t>:: operator()
        (function                  &F,
         samples                   &Samples,
         array<real_t>             &Aorg,
         const array<bool>         &Used,
         array<real_t>             &Aerr,
         callback                  *cb
         )
        {
            
            static const real_t lambda_min    = numeric<real_t>::ftol / 10;
            static const real_t lambda_max    = Pow(REAL(10.0),Floor(Log10(numeric<real_t>::maximum)));
            static const real_t lambda_ini    = REAL(1.0e-3);
            static const real_t lambda_growth = REAL(10.0);
            static const real_t lambda_shrink = REAL(0.1);
            
            
            //__________________________________________________________________
            //
            //
            // initialize
            //
            //__________________________________________________________________
            
            proc = &F;
            data = &Samples;
            nums = data->size();
            if(nums<=0)
                throw exception("%sno sample",fn);
            
            nvar = Aorg.size();
            if(nvar<=0)
                throw exception("%sno variable", fn);
            
            assert(Aorg.size()==Used.size());
            assert(Aerr.size()==Aorg.size());
            
            mkl::set(Aerr,0);
            //__________________________________________________________________
            //
            //
            // compute degrees of freedom
            //
            //__________________________________________________________________
            
            size_t ntot = 0;
            for(size_t i=nums;i>0;--i)
            {
                sample &S = *Samples[i];
                if(S.dFda.size() != nvar )
                    S.prepare(nvar);
                ntot += S.N;
            }
            if(ntot<=0) throw exception("%sno registerd data",fn);
            
            size_t dof = ntot;
            for(size_t i=nvar;i>0;--i)
            {
                if(Used[i])
                {
                    if(dof<=0)
                        throw exception("%snot enough data",fn);
                    --dof;
                }
            }
            
            //__________________________________________________________________
            //
            //
            // extra memory
            //
            //__________________________________________________________________
            cleanup();
            alpha.make(nvar,nvar);
            curv. make(nvar,nvar);
            beta. make(nvar,0);
            step. make(nvar,0);
            aorg. make(nvar,0);
            used. make(nvar,true);
            atmp. make(nvar,0);
            LU.   make(nvar,0);
            numeric<real_t>::function probe( this, & least_squares<real_t>::compute_D2_tmp);
            
            //__________________________________________________________________
            //
            //
            // transfer data
            //
            //__________________________________________________________________
            for(size_t k=nvar;k>0;--k)
            {
                aorg[k] = Aorg[k];
                used[k] = Used[k];
            }
            
            
            //__________________________________________________________________
            //
            //
            // starting point
            //
            //__________________________________________________________________
            ftol                        = Fabs(ftol);
            real_t               lambda = lambda_ini;
            least_squares_result ans    = least_squares_success;
            
        SEARCH:
            D2 = compute_D2_org();
            if(verbose) std::cerr << fn << "aorg   = " << aorg << std::endl;
            if(verbose) std::cerr << fn << "D2     = " << D2    << std::endl;
            
            
            
            //__________________________________________________________________
            //
            //
            // compute approximate curvature
            //
            //__________________________________________________________________
            while( !build_curvature(lambda) )
            {
                lambda = max_of(lambda,lambda_min) * lambda_growth;
                if( lambda > lambda_max )
                {
                    if(verbose) std::cerr << fn << "<singular parameters>" << std::endl;
                    return least_squares_failure;
                }
            }
            
            //__________________________________________________________________
            //
            //
            // compute full Newton's step
            //
            //__________________________________________________________________
            mkl::set(step,beta);
            crout<real_t>::solve(curv,step);
            
            //__________________________________________________________________
            //
            //
            // detect where we are
            //
            //__________________________________________________________________
            const real_t D2_tmp = probe(1);
            
            if(D2_tmp<D2)
            {
                //______________________________________________________________
                //
                // check convergence for variables
                //______________________________________________________________
                bool converged = true;
                for(size_t k=nvar;k>0;--k)
                {
                    const real_t da = Fabs(aorg[k]-atmp[k]);
                    if( da > ftol * Fabs( atmp[k] ))
                    {
                        converged = false;
                        break;
                    }
                }
                
                //______________________________________________________________
                //
                // check convergence for D2
                //______________________________________________________________
                if(!converged)
                {
                    if( Fabs(D2_tmp-D2) <= ftol * Fabs(D2) )
                        converged = true;
                }
                
                //______________________________________________________________
                //
                // accept the full step and decrease lambda
                //______________________________________________________________
                
                mkl::set(aorg,atmp);
                mkl::set(Aorg,aorg);
#define APPLY_CALLBACK() do { if( cb && ! ((*cb)(F,Samples)) ) return least_squares_failure; } while(false)
                
                APPLY_CALLBACK();
                
                if(converged)
                {
                    if(verbose) std::cerr << fn << "<converged>" << std::endl;
                    goto CONVERGED;
                }
                
                lambda *= lambda_shrink;
                if(lambda<lambda_min)
                {
                    lambda = 0;
                }
                
            }
            else
            {
                //______________________________________________________________
                //
                // numeric roundoff is likely: try a local minization
                //______________________________________________________________
                triplet<real_t> xx = { 0,  0, 1      };
                triplet<real_t> ff = { D2, 0, D2_tmp };
                
                if( bracket<real_t>::inside(probe, xx, ff) )
                {
                    
                    minimize(probe, xx, ff, ftol);
                    const real_t D2_old = D2;
                    D2 = probe(xx.b);
                    mkl::set(aorg,atmp);
                    mkl::set(Aorg,aorg);
                    if(D2>=D2_old)
                    {
                        if(verbose) std::cerr << fn << "<spurious level-1>" << std::endl;
                        ans = least_squares_spurious;
                        goto CONVERGED;
                    }
                    APPLY_CALLBACK();
                }
                
                
                // increase lambda and go back
                lambda = max_of(lambda,lambda_min) * lambda_growth;
                if(lambda>lambda_max)
                {
                    if(verbose) std::cerr << fn << "<spurious level-2>" << std::endl;
                    ans = least_squares_spurious;
                    goto CONVERGED;
                }
            }
            goto SEARCH;
            
        CONVERGED:
            D2 = compute_D2_org();
            APPLY_CALLBACK();
            if(verbose) std::cerr << fn << "aorg   = " << aorg << std::endl;
            if(verbose) std::cerr << fn << "D2     = " << D2   << std::endl;
            
            //__________________________________________________________________
            //
            // compute errors by covariance matrix
            //__________________________________________________________________
            
            if( ! LU.build(alpha) )
            {
                throw exception("%sunexpected singular curvature", fn);
            }
            curv.ld1();
            crout<real_t>::solve(alpha,curv);
            
            if(verbose) std::cerr << fn << "#dof   = " << dof    << std::endl;
            if(dof>0)
            {
                const real_t residue = D2/dof;
                for(size_t k=nvar;k>0;--k)
                {
                    if(Used[k])
                    {
                        Aerr[k] = Sqrt( residue * Fabs(curv[k][k]) );
                    }
                }
            }
            
            
            return ans;
        }
        
        
    }
}
