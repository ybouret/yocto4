#include "yocto/math/ztype.hpp"
#include "yocto/math/fit/gnl-lsf.hpp"
#include "yocto/math/kernel/tao.hpp"


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
        Q(0),
        M(0),
        Gamma(),
        u(),
        dFdu(),
        beta(),
        alpha(),
        Alpha(),
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
			(size_t &)Q = local_nvar;
			(size_t &)M = global_nvar;
            
			Gamma .make(Q,M);
			u     .make(Q,0);
			dFdu  .make(Q,0);
			beta  .make(Q,0);
			alpha .make(Q,Q);
			__ag  .make(Q,M);
			Alpha  .make(M,M);
            
            if(Q==M)
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
		real_t LeastSquares<real_t>::Sample:: compute_D( Function &F, const Array &a)
		{
            //__________________________________________________________________
            //
			// Evaluate u from a
            //__________________________________________________________________
			tao::mul(u,Gamma,a);
            
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
				w.x = Xi;
				for(size_t q=Q;q>0;--q)
				{
					w.q = q;
					const real_t g = drvs(f,u[q],h);
					dFdu[q]  = g;
					beta[q] += d * g;
				}
                
				for(size_t j=Q;j>0;--j)
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
			for(size_t j=Q;j>0;--j)
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
			return ans;
		}
        
        
		template <>
		void  LeastSquares<real_t>::Sample::collect( Matrix &user_alpha, Array &user_beta ) const throw()
		{
			//std::cerr << "Alpha.rows=" << Alpha.rows <<  ", M=" << M << std::endl;
			//std::cerr << "Alpha.cols=" << Alpha.cols <<  ", M=" << M << std::endl;
			assert(user_alpha.rows == M);
			assert(user_alpha.cols == M);
			assert(user_beta.size() >= Q);
			tao::mul_add_trn(user_beta,Gamma,beta);
			for(size_t i=M;i>0;--i)
			{
				for(size_t j=M;j>0;--j)
				{
					user_alpha[i][j] += Alpha[i][j];
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
        
        
	}
    
}

#include "yocto/math/kernel/crout.hpp"
#include "yocto/code/ipower.hpp"
#include "yocto/math/opt/bracket.hpp"
#include "yocto/math/opt/minimize.hpp"

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
            tao::setprobe(atmp, aorg, x, step);
            real_t ans(0);
            for(size_t k=ns;k>0;--k)
            {
                Sample &s = *(*S)[k];
                ans += s.compute_D(*F,atmp);
            }
            return ans;
        }
        
		template <>
		LeastSquares<real_t>:: LeastSquares() :
		drvs(),
        scan(this, & LeastSquares<real_t>::evalD),
        h(REAL(1e-4))
		{
		}
        
		template <>
		real_t LeastSquares<real_t>:: computeD()
		{
			real_t ans(0);
            
            // full evaluation
			tao::ld(beta,0);
			alpha.ldz();
			for(size_t k=ns;k>0;--k)
			{
				Sample &s = *(*S)[k];
				ans += s.compute_D(*F,aorg,drvs,h);
				s.collect(alpha,beta);
			}
            
            // alpha and beta correction
			for(size_t i=nvar;i>0;--i)
			{
				if( !used[i] )
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
        fit_status LeastSquares<real_t>:: operator()(
                                                     Samples           &user_S,
                                                     Function          &user_F,
                                                     Array             &user_aorg,
                                                     const array<bool> &user_used,
                                                     Array             &user_aerr)
        {
            static const int    LAMBDA_INI_POW10 =  LAMBDA_MIN_POW10/2;
            
            //__________________________________________________________________
            //
            // prepare variables
            //__________________________________________________________________
            S    = &user_S;
            F    = &user_F;
            ns   = S->size();
            nvar = user_aorg.size();
            assert(user_used.size()>=nvar);
            assert(user_aerr.size()>=nvar);
            
            //__________________________________________________________________
            //
            // allocate memory
            //__________________________________________________________________
            aorg  .make(nvar,0);
            used  .make(nvar,true);
            atmp  .make(nvar,0);
            aerr  .make(nvar,0);
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
                aorg[i] = user_aorg[i];
                used[i] = user_used[i];
                aerr[i] = user_aerr[i];
            }
            int p = LAMBDA_INI_POW10;
            
            
            //__________________________________________________________________
            //
            // starting point
            //__________________________________________________________________
            real_t Dorg  = computeD();
            size_t count = 0;
            std::cerr << "Dorg ="  << Dorg  << std::endl;
            std::cerr << "alpha="  << alpha << std::endl;
            std::cerr << "beta ="  << beta  << std::endl;
            
            
            
            //__________________________________________________________________
            //
            // for a given alpha matrix
            //__________________________________________________________________
            fit_status result = fit_failure;
        UPDATE_LAMBDA:
            ++count;
            if( !build_curvature( compute_lam(p) ) )
            {
                ++p;
                if(p>LAMBDA_MAX_POW10)
                {
                    std::cerr << "singular" << std::endl;
                    return result;
                }
                goto UPDATE_LAMBDA;
            }
            std::cerr << "p=" << p << std::endl;
            
            //__________________________________________________________________
            //
            // compute step
            //__________________________________________________________________
            tao::set(step,beta);
            crout<real_t>::solve(curv,step);
            std::cerr << "aorg=" << aorg << std::endl;
            std::cerr << "beta=" << beta << std::endl;
            std::cerr << "step=" << step << std::endl;
            
            //__________________________________________________________________
            //
            // compute full step trial
            //__________________________________________________________________
            const real_t Dtmp = evalD(1.0);
            std::cerr << "Dtmp=" << Dtmp << std::endl;
            
            //__________________________________________________________________
            //
            // take action
            //__________________________________________________________________
            if(Dtmp<Dorg)
            {
                //______________________________________________________________
                //
                // Don't go too fast
                //______________________________________________________________
                triplet<real_t> XX = { 0,    1,    1    };
                triplet<real_t> DD = { Dorg, Dtmp, Dtmp };
                if( bracket<real_t>::inside(scan, XX, DD) )
                {
                    minimize<real_t>(scan, XX, DD, 0);
                }
                else
                {
                    XX.b = 1.0;
                    DD.b = Dtmp;
                }
                
                //______________________________________________________________
                //
                // successfull step
                //______________________________________________________________
                p = max_of(--p,LAMBDA_MIN_POW10);
                for(size_t i=nvar;i>0;--i)
                {
                    atmp[i]  = aorg[i];
                    aorg[i] += XX.b * step[i];
                    atmp[i] -= aorg[i];
                }
                std::cerr << "diff=" << atmp << std::endl;
                const real_t Dnew = computeD();
                
                
                //______________________________________________________________
                //
                // Test convergence on values
                //______________________________________________________________
                bool cvg = true;
                for(size_t i=nvar;i>0;--i)
                {
                    if( Fabs(atmp[i])>0 )
                    {
                        cvg = false;
                        break;
                    }
                }
                
                if(cvg)
                {
                    std::cerr << "SUCCESS: variables numerically converged" << std::endl;
                    result = fit_success;
                    goto COMPUTE_ERROR;
                }
                
                Dorg = Dnew;
                goto UPDATE_LAMBDA;
            }
            else
            {
                //______________________________________________________________
                //
                // must increase lambda, from the same starting point
                //______________________________________________________________
                std::cerr << "must decrease step" << std::endl;
                const real_t dD = Fabs(Dorg-Dtmp);
                
                if(dD<=0 || ++p>LAMBDA_MAX_POW10)
                {
                    std::cerr << "spurious" << std::endl;
                    result = fit_spurious;
                    goto COMPUTE_ERROR;
                    
                }
                
                goto UPDATE_LAMBDA;
            }
            
        COMPUTE_ERROR:
            
            return result;
            
        }
        
        
        
        
        
    }
}
