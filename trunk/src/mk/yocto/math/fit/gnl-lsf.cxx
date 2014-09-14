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
			Array       &userZ) throw() :
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
			curv(),
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
			curv  .make(M,M);
		}

		template <>
		void LeastSquares<real_t>::Sample::prepare(size_t nvar)
		{
			prepare(nvar,nvar);
			Gamma.ld1();
		}

		template <>
		real_t LeastSquares<real_t>::Sample:: compute_D( Function &F, const Array &a)
		{
			// Evaluate u from a
			tao::mul(u,Gamma,a); 

			// run
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
			// Evaluate u from a
			tao::mul(u,Gamma,a);

			// initialize
			w.F = &F;
			tao::ld(beta,REAL(0.0));
			alpha.ldz();

			// accumulate
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

			// symmetric part
			for(size_t j=Q;j>0;--j)
			{
				for(size_t k=j-1;k>0;--k)
				{
					alpha[k][j] = alpha[j][k];
				}
			}

			// compute alpha * Gamma
			tao::mmul(__ag,alpha,Gamma);

			// compute curv = Gamma'*alpha
			tao::mmul_ltrn(curv,Gamma,__ag);
			return ans;
		}


		template <>
		void  LeastSquares<real_t>::Sample::collect( Matrix &Alpha, Array &Beta ) const throw()
		{
			assert(Alpha.rows == M);
			assert(Alpha.cols == M);
			assert(Beta.size() >= Q);
			tao::mul_add_trn(Beta,Gamma,beta);
			for(size_t i=M;i>0;--i)
			{
				for(size_t j=M;j>0;--j)
				{
					Alpha[i][j] += curv[i][j];
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
		LeastSquares<real_t>::Sample & LeastSquares<real_t>:: Samples:: append(const Array &X, const Array &Y, Array &Z)
		{
			Sample::Pointer ptr( new Sample(X,Y,Z) );
			push_back(ptr);
			return *ptr;
		}

	}

}


namespace yocto
{
	namespace math
	{



		template <>
		LeastSquares<real_t>:: ~LeastSquares() throw()
		{
		}

		template <>
		LeastSquares<real_t>:: LeastSquares() :
		drvs(),
			h(REAL(1e-4))
		{
		}

		template <>
		void LeastSquares<real_t>:: fit(
			Samples           &user_S,
			Function          &user_F,
			Array             &user_aorg,
			const array<bool> &user_used,
			Array             &user_aerr)
		{
			// prepare variables
			S    = &user_S;
			F    = &user_F;
			ns   = S->size();
			nvar = aorg.size();
			assert(user_used.size()>=nvar);
			assert(user_aerr.size()>=nvar);

			// allocate memory
			aorg.make(nvar,0);
			used.make(nvar,true);
			aerr.make(nvar,0);
			beta.make(nvar,0);
			curv.make(nvar,nvar);
			step.make(nvar,0);

			// initialize local variables
			for(size_t i=nvar;i>=0;--i)
			{
				aorg[i] = user_aorg[i];
				used[i] = user_used[i];
				aerr[i] = user_aerr[i];
			}

			// starting point
			real_t Dorg = computeD();
		}

		template <>
		real_t LeastSquares<real_t>:: computeD()
		{
			real_t ans(0);
			tao::ld(beta,0);
			curv.ldz();
			for(size_t k=ns;k>0;--k)
			{
				Sample &s = *(*S)[k];
				ans += s.compute_D(*F,aorg,drvs,h);
				s.collect(curv,beta);
			}
			return ans;
		}

	}
}
