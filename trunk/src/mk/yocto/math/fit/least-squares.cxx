#include "yocto/math/ztype.hpp"
#include "yocto/math/fit/least-squares.hpp"
#include "yocto/exception.hpp"
#include "yocto/math/kernel/algebra.hpp"
#include "yocto/code/utils.hpp"

#include "yocto/math/opt/bracket.hpp"



namespace yocto
{
    namespace math
    {
        
        typedef algebra<real_t> mkl;
        
        
        ////////////////////////////////////////////////////////////////////
        //
        // sample operations
        //
        ////////////////////////////////////////////////////////////////////
        
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
                        const double g = dFda[k];
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
        
        
        ////////////////////////////////////////////////////////////////////
        //
        // samples operations
        //
        ////////////////////////////////////////////////////////////////////
        
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
                ans += S.compute_D2(*proc, *aorg, *used, drvs, h);
                
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
                if( ! (*used)[k] ) alpha[k][k] = REAL(1.0);
            }
            
            return ans;
        }
        
        
        template <>
        least_squares<real_t>:: least_squares() :
        D2(0),
        drvs(),
        h(1e-4),
        ftol(numeric<real_t>::ftol),
        verbose(false),
        proc(0),
        data(0),
        nvar(0),
        nums(0),
        alpha(),
        beta(),
        step(),
        aorg(0),
        used(0),
        LU()
        {
            
        }
        
        
        template <>
        bool least_squares<real_t>:: build_curvature(real_t lambda)
        {
            if(verbose) std::cerr << "[least_squares] lambda=" << lambda << std::endl;
            const real_t fac  = REAL(1.0) + lambda;
            for(size_t k=nvar;k>0;--k)
            {
                for(size_t l=nvar;l>0;--l)
                {
                    const double aa = alpha[k][l];
                    curv[k][l] = (k == l) ? fac * aa : aa;
                }
            }
            
            return LU.build(curv);
        }
        
        template <>
        real_t least_squares<real_t>:: compute_D2_tmp()
        {
            
            for(size_t k=nvar;k>0;--k)
            {
                atmp[k] = (*aorg)[k] + step[k];
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
            static const char fn[]   = "least_squares";
            
            static const real_t lambda_min    = numeric<double>::ftol / 10;
            static const real_t lambda_max    = Pow(REAL(10.0),Floor(Log10(numeric<real_t>::maximum)));
            static const real_t lambda_ini    = 1e-3;
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
                throw exception("[%s] no sample",fn);
            
            nvar = Aorg.size();
            if(nvar<=0)
                throw exception("[%s] no variable", fn);
            
            assert(Aorg.size()==Used.size());
            assert(Aerr.size()==Aorg.size());
            
            aorg = &Aorg;
            used = &Used;
            mkl::set(Aerr,REAL(0.0));
            
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
            if(ntot<=0) throw exception("[%s] no registerd data",fn);
            
            size_t dof = ntot;
            for(size_t i=nvar;i>0;--i)
            {
                if(Used[i])
                {
                    if(dof<=0)
                        throw exception("[%s] not enough data",fn);
                    --dof;
                }
            }
            
            //__________________________________________________________________
            //
            //
            // extra memory
            //
            //__________________________________________________________________
            
            alpha.make(nvar,nvar);
            curv. make(nvar,nvar);
            beta. make(nvar,0);
            step. make(nvar,0);
            atmp. make(nvar,0);
            LU.   make(nvar,0);
            
            
            
            
            //__________________________________________________________________
            //
            //
            // starting point
            //
            //__________________________________________________________________
            ftol = Fabs(ftol);
            real_t lambda = lambda_ini;
            least_squares_result ans = least_squares_success;
            
        SEARCH:
            D2 = compute_D2_org();
            if(verbose) std::cerr << "[least_squares] aorg   = " << *aorg << std::endl;
            if(verbose) std::cerr << "[least_squares] D2     = " << D2    << std::endl;
            
           
            
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
                    if(verbose) std::cerr << "[least_squares] singular point" << std::endl;
                    return least_squares_failure;
                }
            }
            
            //__________________________________________________________________
            //
            //
            // use beta as full Newton's step
            //
            //__________________________________________________________________
            mkl::set(step,beta);
            crout<real_t>::solve(curv,step);
            std::cerr << "alpha=" << alpha << std::endl;
            std::cerr << "beta="  << beta  << std::endl;
            std::cerr << "step="  << step  << std::endl;
            
            //__________________________________________________________________
            //
            //
            // detect where we are
            //
            //__________________________________________________________________
            const real_t D2_tmp = compute_D2_tmp();
            
            if(D2_tmp<D2)
            {
                //______________________________________________________________
                //
                // check convergence for variables
                //______________________________________________________________
                bool converged = true;
                for(size_t k=nvar;k>0;--k)
                {
                    const real_t da = Fabs((*aorg)[k]-atmp[k]);
                    if( da > ftol * Fabs( atmp[k] ))
                    {
                        converged = false;
                        break;
                    }
                }
                
                if(!converged)
                {
                    if( Fabs(D2_tmp-D2) <= ftol * Fabs(D2) )
                        converged = true;
                }
                
                //______________________________________________________________
                //
                // accept the full step and decrease lambda
                //______________________________________________________________
                
                mkl::set(*aorg,atmp);
                if( cb && ! ((*cb)(F,Samples)) )
                {
                    return least_squares_failure;
                }

                if(converged)
                {
                    if(verbose) std::cerr << "[least_squares] converged" << std::endl;
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
                // numeric roundoff is likely
                std::cerr << "Too Big!!!" << std::endl;
                
                // increase lambda and go back
                lambda = max_of(lambda,lambda_min) * lambda_growth;
                if(lambda>lambda_max)
                {
                    if(verbose) std::cerr << "[least_squares] spurious point" << std::endl;
                    ans = least_squares_spurious;
                    goto CONVERGED;
                }
            }
            goto SEARCH;
            
        CONVERGED:
            D2 = compute_D2_org();
            if( cb && ! ((*cb)(F,Samples)) )
            {
                return least_squares_failure;
            }
            if(verbose) std::cerr << "[least_squares] aorg   = " << *aorg << std::endl;
            if(verbose) std::cerr << "[least_squares] D2     = " << D2    << std::endl;
            
            if( ! LU.build(alpha) )
            {
                throw exception("[%s] unexpected singular curvature", fn);
            }
            curv.ld1();
            crout<real_t>::solve(alpha,curv);
            
            if(verbose) std::cerr << "[least_squares] #dof   = " << dof    << std::endl;
            if(dof>0)
            {
                const real_t residue = D2/dof;
                for(size_t k=nvar;k>0;--k)
                {
                    if(Used[k])
                    {
                        Aerr[k] = Sqrt( residue * curv[k][k]);
                    }
                }
            }
            
            
            return ans;
        }
        
        
        
    }
}
