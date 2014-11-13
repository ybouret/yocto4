#include "yocto/math/ztype.hpp"
#include "yocto/math/fit/lsf.hpp"
#include "yocto/math/kernel/tao.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace math
    {
        template <>
        LeastSquares<real_t>::Sample:: Wrapper:: ~Wrapper() throw() {}
        
        template <>
        LeastSquares<real_t>::Sample:: Wrapper:: Wrapper(Sample &from) throw():
        S(from),F(0),x(0),q(0)
        {
            
        }
        
        
        template <>
        real_t LeastSquares<real_t>::Sample:: Wrapper:: Eval( real_t U )
        {
            assert(q>0);
            assert(F!=0);
            assert(S.u.size()>=q);
            Array &u = S.u;
            const real_t u_save = u[q];
            u[q] = U;
            const real_t ans = (*F)(x,u);
            u[q] = u_save;
            return ans;
        }
    }
    
}



namespace yocto
{
    namespace math
    {
        
        template <>
        LeastSquares<real_t>::Sample:: ~Sample() throw()
        {
        }
        
#if defined(_MSC_VER)
#pragma warning ( disable : 4355 )
#endif
        
        template <>
        LeastSquares<real_t>::Sample:: Sample(const Array &userX,
                                              const Array &userY,
                                              Array       &userZ) :
        X( userX ),
        Y( userY ),
        Z( userZ ),
        N(0),
        L(0),
        M(0),
        Gamma(),
        u(),
        dFdu(),
        beta(),
        alpha(),
        __ag(),
        Alpha(),
        D(0),
        w(*this),
        f(&w,&Wrapper::Eval)
        {
        }
        
        template <>
        void LeastSquares<real_t>::Sample:: prepare(size_t local_nvar, size_t global_nvar)
        {
            assert(local_nvar>0);
            assert(global_nvar>0);
            assert(local_nvar<=global_nvar);
            assert( X.size() == Y.size() );
            assert( X.size() == Z.size() );
            (size_t &)N = X.size();
            if(local_nvar>N)
            {
                throw exception("too many variables");
            }
            (size_t &)L = local_nvar;
            (size_t &)M = global_nvar;
            
            Gamma .make(L,M);
            u     .make(L,0);
            dFdu  .make(L,0);
            beta  .make(L,0);
            alpha .make(L,L);
            __ag  .make(L,M);
            Alpha .make(M,M);
            
            if(L==M)
            {
                Gamma.ld1();
            }
        }
        
        template <>
        void LeastSquares<real_t>::Sample::prepare(size_t nvar)
        {
            prepare(nvar,nvar);
        }
        
        template <>
        real_t LeastSquares<real_t>::Sample:: compute_D( Function &F, const Array &a) const
        {
            //__________________________________________________________________
            //
            // Evaluate u from a
            //__________________________________________________________________
            tao::mul((Vector&)u,Gamma,a);
            
            //__________________________________________________________________
            //
            // run
            //__________________________________________________________________
            real_t ans = 0;
            for(size_t i=N;i>0;--i)
            {
                Z[i] = F(X[i],u);
                const real_t d = Y[i] - Z[i];
                ans += d*d;
            }
            return ans;
        }
        
        template <>
        real_t LeastSquares<real_t>::Sample:: compute_D(Function &F,const Array &a, derivative<real_t> &drvs, real_t h)
        {
            //__________________________________________________________________
            //
            // Evaluate u from a
            //__________________________________________________________________
            tao::mul(u,Gamma,a);
            
            //__________________________________________________________________
            //
            // initialize
            //__________________________________________________________________
            w.F = &F;
            tao::ld(beta,REAL(0.0));
            alpha.ldz();
            
            //__________________________________________________________________
            //
            // accumulate
            //__________________________________________________________________
            
            real_t ans = 0;
            for(size_t i=N;i>0;--i)
            {
                const real_t Xi = X[i];
                Z[i]            = F(Xi,u);
                const real_t d = Y[i] - Z[i];
                //______________________________________________________________
                //
                // compute gradient @Xi
                //______________________________________________________________
                w.x = Xi;
                for(size_t q=L;q>0;--q)
                {
                    w.q = q;
                    const real_t g = drvs(f,u[q],h);
                    dFdu[q]  = g;
                    beta[q] += d * g;
                }
                
                for(size_t j=L;j>0;--j)
                {
                    for(size_t k=j;k>0;--k)
                    {
                        alpha[j][k] += dFdu[j] * dFdu[k];
                    }
                }
                
                ans += d*d;
            }
            
            //__________________________________________________________________
            //
            // symmetric part
            //__________________________________________________________________
            for(size_t j=L;j>0;--j)
            {
                for(size_t k=j-1;k>0;--k)
                {
                    alpha[k][j] = alpha[j][k];
                }
            }
            
            //__________________________________________________________________
            //
            // compute alpha * Gamma
            //__________________________________________________________________
            tao::mmul(__ag,alpha,Gamma);
            
            //__________________________________________________________________
            //
            // compute curv = Gamma'*alpha
            //__________________________________________________________________
            tao::mmul_ltrn(Alpha,Gamma,__ag);
            
            D = ans;
            return ans;
        }
        
        
        template <>
        void  LeastSquares<real_t>::Sample::collect( Matrix &user_alpha, Array &user_beta ) const throw()
        {
            assert(user_alpha.rows  == M);
            assert(user_alpha.cols  == M);
            assert(user_beta.size() >= L);
            tao::mul_add_trn(user_beta,Gamma,beta);
            for(size_t i=M;i>0;--i)
            {
                for(size_t j=M;j>0;--j)
                {
                    user_alpha[i][j] += Alpha[i][j];
                }
            }
        }
        
        template <>
        void LeastSquares<real_t>::Sample:: connect( size_t local_ivar, size_t global_ivar ) throw()
        {
            assert(local_ivar  >  0);
            assert(global_ivar >  0);
            assert(local_ivar  <= global_ivar);
            assert(local_ivar  <= L);
            assert(global_ivar <= M);
            for(size_t j=1;j<=M;++j) Gamma[local_ivar][j] = 0;
            Gamma[local_ivar][global_ivar] = 1;
        }
        
        
    }
    
}

namespace yocto
{
    namespace math
    {
        
        
        template <>
        LeastSquares<real_t>:: Samples:: Samples() throw() {}
        
        template <>
        LeastSquares<real_t>:: Samples:: ~Samples() throw() {}
        
        template <>
        LeastSquares<real_t>:: Sample & LeastSquares<real_t>:: Samples:: append(const Array &X, const Array &Y, Array &Z)
        {
            Sample::Pointer ptr( new Sample(X,Y,Z) );
            push_back(ptr);
            return *ptr;
        }
        
        template <>
        void LeastSquares<real_t>:: Samples:: prepare(size_t local_nvar, size_t global_nvar)
        {
            for(size_t i=size();i>0;--i)
            {
                (*(*this)[i]).prepare(local_nvar, global_nvar);
            }
        }
        
        template <>
        void LeastSquares<real_t>:: Samples:: prepare(size_t nvar)
        {
            prepare(nvar, nvar);
        }
        
        template <>
        real_t  LeastSquares<real_t>:: Samples:: computeD(Function &F, const Array &a) const
        {
            real_t D = 0;
            for(size_t i=size();i>0;--i)
            {
                const Sample &sample = *(*this)[i];
                D += sample.compute_D(F,a);
            }
            return D;
        }
        
        template <>
        real_t  LeastSquares<real_t>:: Samples::corr() const
        {
            size_t n = 0;
            real_t mean_y = 0;
            real_t mean_z = 0;
            
            for(size_t i=size();i>0;--i)
            {
                const Sample &sample = *(*this)[i];
                const size_t  N = sample.N;
                n += N;
                for(size_t j=N;j>0;--j)
                {
                    mean_y += sample.Y[j];
                    mean_z += sample.Z[j];
                }
            }
            if(n<=1)
            {
                throw exception("Samples correlation: no data!");
            }
            mean_y /= n;
            mean_z /= n;
            
            real_t sum_dy2 = 0;
            real_t sum_dz2 = 0;
            real_t sum_dyz = 0;
            
            for(size_t i=size();i>0;--i)
            {
                const Sample &sample = *(*this)[i];
                const size_t  N = sample.N;
                for(size_t j=N;j>0;--j)
                {
                    const real_t dy = sample.Y[j] - mean_y;
                    const real_t dz = sample.Z[j] - mean_z;
                    sum_dy2 += dy*dy;
                    sum_dz2 += dz*dz;
                    sum_dyz += dy*dz;
                }
            }
            const real_t sig_y = Sqrt(sum_dy2/n);
            const real_t sig_z = Sqrt(sum_dz2/n);
            const real_t covyz = sum_dyz / n;
            const real_t den   = (sig_y * sig_z + numeric<real_t>::tiny);
            return covyz/den;
        }
        
        
    }
    
}

#include "yocto/math/kernel/crout.hpp"
#include "yocto/code/ipower.hpp"

#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"

#include "yocto/math/kernel/jacobi.hpp"

namespace yocto
{
    namespace math
    {
        
        
        
        template <>
        LeastSquares<real_t>:: ~LeastSquares() throw()
        {
        }
        
        template <>
        real_t LeastSquares<real_t>:: evalD(real_t x)
        {
            tao::setprobe(Atmp, Aorg, x, step);
            return S->computeD(*F,Atmp);
        }
        
        template <>
        LeastSquares<real_t>:: LeastSquares() :
        S(0),
        F(0),
        ns(0),
        nvar(0),
        Aorg(),
        Used(),
        Atmp(),
        beta(),
        alpha(),
        curv(),
        step(),
        drvs(),
        scan(this, & LeastSquares<real_t>::evalD),
        h(REAL(1e-4)),
        verbose(false),
        fast(false)
        {
        }
        
        template <>
        real_t LeastSquares<real_t>:: computeD()
        {
            real_t ans(0);
            
            //__________________________________________________________________
            //
            // full evaluation
            //__________________________________________________________________
            tao::ld(beta,0);
            alpha.ldz();
            for(size_t k=ns;k>0;--k)
            {
                Sample &s = *(*S)[k];
                ans += s.compute_D(*F,Aorg,drvs,h);
                s.collect(alpha,beta);
            }
            
            //__________________________________________________________________
            //
            // alpha and beta correction
            //__________________________________________________________________
            for(size_t i=nvar;i>0;--i)
            {
                if( !Used[i] )
                {
                    beta[i] = 0.0;
                    for(size_t j=nvar;j>0;--j)
                    {
                        alpha[i][j] = 0;
                        alpha[j][i] = 0;
                    }
                    alpha[i][i] = 1.0;
                }
            }
            
            return ans;
        }
        
        
        
        template <>
        bool LeastSquares<real_t>:: build_curvature(real_t lam)
        {
            const real_t fac = REAL(1.0) + lam;
            curv.assign(alpha);
            for(size_t i=nvar;i>0;--i)
            {
                curv[i][i] *= fac;
            }
            return crout<real_t>::build(curv);
        }
        
        
        
        template <>
        const int LeastSquares<real_t>:: LAMBDA_MIN_POW10 = int(Floor(Log10(numeric<real_t>::epsilon)));
        
        template <>
        const int LeastSquares<real_t>:: LAMBDA_MAX_POW10 = int(Ceil(Log10(numeric<real_t>::maximum)))/2;
        
        template<>
        real_t  LeastSquares<real_t>::  compute_lam(int p)  throw()
        {
            if(p<=LAMBDA_MIN_POW10)
            {
                return 0;
            }
            else
            {
                return (p < 0) ? ipower(REAL(0.1),-p) : ipower(REAL(10.0),p);
            }
        }
        
        
        
        template <>
        bool LeastSquares<real_t>:: operator()(Samples           &user_S,
                                               Function          &user_F,
                                               Array             &aorg,
                                               const array<bool> &used,
                                               Array             &aerr,
                                               Callback          *cb)
        {
            static const int    LAMBDA_INI_POW10 =  LAMBDA_MIN_POW10/2;
            
            //__________________________________________________________________
            //
            // prepare variables
            //__________________________________________________________________
            S    = &user_S;
            F    = &user_F;
            ns   = S->size();
            nvar = aorg.size();
            assert(used.size()>=nvar);
            assert(aerr.size()>=nvar);
            
            size_t ndata = 0;
            for(size_t i=ns;i>0;--i)
            {
                ndata += (*S)[i]->N;
            }
            size_t param = nvar;
            for(size_t i=nvar;i>0;--i)
            {
                if(!used[i])
                    --param;
            }
            if(param>ndata)
            {
                throw exception("Fit: not enough #data=%u for #param=%u", unsigned(ndata), unsigned(param));
            }
            
            //__________________________________________________________________
            //
            // allocate memory
            //__________________________________________________________________
            Aorg  .make(nvar,0);
            Used  .make(nvar,true);
            Atmp  .make(nvar,0);
            beta  .make(nvar,0);
            alpha .make(nvar,nvar);
            curv  .make(nvar,nvar);
            step  .make(nvar,0);
            
            //__________________________________________________________________
            //
            // initialize local variables
            //__________________________________________________________________
            for(size_t i=nvar;i>0;--i)
            {
                Aorg[i] = aorg[i];
                Used[i] = used[i];
            }
            int p = LAMBDA_INI_POW10;
            
            
#define Y_LSF_CB() do { if(cb && !(*cb)(*F,*S) ) return false; } while(false)
            //__________________________________________________________________
            //
            // starting point
            //__________________________________________________________________
            real_t Dorg  = computeD();
            size_t count = 0;
            if(verbose)
            {
                std::cerr << "#init" << std::endl;
                std::cerr << "\ta=" << Aorg << std::endl;
                std::cerr << "\tD=" << Dorg << std::endl;
            }
            
            
            //__________________________________________________________________
            //
            // loop
            //__________________________________________________________________
        UPDATE_LAMBDA:
            Y_LSF_CB();
            //__________________________________________________________________
            //
            // Find an acceptable curvature
            //__________________________________________________________________
            if( !build_curvature( compute_lam(p) ) )
            {
                ++p;
                if(p>LAMBDA_MAX_POW10)
                {
                    if(verbose)
                    {
                        std::cerr << "#Fit: singular system" << std::endl;
                    }
                    return false;
                }
                goto UPDATE_LAMBDA;
            }
            
            //__________________________________________________________________
            //
            // compute step
            //__________________________________________________________________
            tao::set(step,beta);
            crout<real_t>::solve(curv,step);
            
            //__________________________________________________________________
            //
            // compute full step trial, keep information to change p
            //__________________________________________________________________
            const real_t Dtmp = evalD(1.0);
            const bool   isOK = Dtmp <= Dorg;
            
            //__________________________________________________________________
            //
            // Line minimize for numerical optimum, may get back !
            //__________________________________________________________________
            triplet<real_t> XX = { 0,    1,    1    };
            triplet<real_t> DD = { Dorg, Dtmp, Dtmp };
            bracket<real_t>::expand(scan,XX,DD);
            minimize<real_t>(scan, XX, DD, 0);
            
            
            //__________________________________________________________________
            //
            // We have it
            //__________________________________________________________________
            for(size_t i=nvar;i>0;--i)
            {
                Atmp[i]  = Aorg[i];
                Aorg[i] += XX.b * step[i];
                Atmp[i] -= Aorg[i];
                aorg[i]  = Aorg[i];
            }
            
            if(verbose)
            {
                std::cerr << "#step " << ++count << ", p=" << p << std::endl;
                std::cerr << "\ta="   << aorg << std::endl;
                std::cerr << "\tD="   << DD.b << std::endl;
            }
            
            //__________________________________________________________________
            //
            // Test convergence on values
            //__________________________________________________________________
            if(fast||isOK)
            {
                //std::cerr << "Testing convergence" << std::endl;
                bool cvg = true;
                for(size_t i=nvar;i>0;--i)
                {
                    if( Fabs(Atmp[i])>0 )
                    {
                        cvg = false;
                        break;
                    }
                }
                
                if(cvg)
                {
                    if( verbose )
                    {
                        std::cerr << "#Fit: variables convergence" << std::endl;
                    }
                    goto COMPUTE_ERROR;
                }
                
                //__________________________________________________________________
                //
                // Test convergence on D, using the minimised value
                //__________________________________________________________________
                if(DD.b>=Dorg)
                {
                    if(verbose)
                    {
                        std::cerr << "#Fit: least squares convergence" << std::endl;
                    }
                    goto COMPUTE_ERROR;
                }
            }
            else
            {
                //std::cerr << "NOT testing convergence" << std::endl;
            }
            //__________________________________________________________________
            //
            // Apply Levenberg-Marquardt update for the full step
            //__________________________________________________________________
            if(!isOK)
            {
                if(++p>LAMBDA_MAX_POW10)
                {
                    if(verbose)
                    {
                        std::cerr << "#Fit: ill conditionned system" << std::endl;
                    }
                    return false;
                }
            }
            else
            {
                p = max_of(--p,LAMBDA_MIN_POW10);
            }
            
            //__________________________________________________________________
            //
            // Full update and go on
            //__________________________________________________________________
            Dorg = computeD();
            goto UPDATE_LAMBDA;
            
            
            
            
        COMPUTE_ERROR:
            //__________________________________________________________________
            //
            //
            // compute errors
            //
            //__________________________________________________________________
            if(verbose)
            {
                std::cerr << "#Fit: computing error estimates" << std::endl;
            }
            Dorg = computeD(); //@aorg
            Y_LSF_CB();
            
            //__________________________________________________________________
            //
            // compute covariance matrix in alpha
            //__________________________________________________________________
            if(!build_curvature(0))
            {
                throw exception("Fit: unexpected singular curvature");
            }
            
            alpha.ld1();
            crout<real_t>::solve(curv, alpha);
            
            //__________________________________________________________________
            //
            // deduce individual errors
            //__________________________________________________________________
            const size_t ndof    = 1 + (ndata-param);
            const real_t residue = Dorg/ndof;
            for(size_t i=nvar;i>0;--i)
            {
                if(Used[i])
                {
                    aerr[i] = Sqrt( residue * Fabs(alpha[i][i]) );
                    aorg[i] = Aorg[i];
                }
                else
                {
                    aerr[i] = 0;
                }
            }
            
            if(verbose)
            {
                for(size_t i=1;i<=nvar;++i)
                {
                    
                    const real_t AA = aorg[i];
                    const real_t AE = aerr[i];
                    std::cerr << "\ta[" << i << "]=" << AA  << " +/-" << AE;
                    if(Fabs(AA)>0)
                    {
                        std::cerr << " (" << (REAL(100.0)*AE/Fabs(AA)) << "%)";
                    }
                    std::cerr << std::endl;
                }
            }
            
            return true;
            
        }
        
        
        
        
        
    }
}
