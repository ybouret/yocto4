#include "yocto/math/stat/dist.hpp"
#include "yocto/math/fcn/functions.hpp"
#include "yocto/math/fcn/zfind.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"

#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto {
	
	namespace math {
		
		template <>
		real_t gaussian<real_t>::pdf( real_t x, const real_t mu, const real_t sig )
		{
			static const real_t sqrt2pi = Sqrt( numeric<real_t>::two_pi );
			
			if( sig <= 0 )
				throw libc::exception( EDOM, "[gaussian::pdf] invalid sigma=%g", sig );
			
			const   real_t X = (x - mu) / sig;
			return  Exp( - (X*X) * real_t(0.5) ) / ( sqrt2pi * sig );
		}
		
		template <>
		real_t gaussian<real_t>::cdf( real_t x, const real_t mu, const real_t sig )
		{
			static const real_t sqrt2 = Sqrt( REAL(2.0) );
			if( sig <= 0 )
				throw libc::exception( EDOM, "[gaussian::cdf] invalid sigma=%g", sig );
			
			return REAL(0.5) * ( REAL(1.0) + qerf( (x-mu) / ( sqrt2 * sig ) ) );
			
		}
		
		
		//-- positive gaussian
		template <>
		real_t gaussian_p<real_t>::pdf( real_t x, const real_t mu, const real_t sig )
		{
			static const real_t sqrt2pi = Sqrt( numeric<real_t>::two_pi );
			static const real_t sqrt2   = Sqrt( REAL(2.0) );
			
			if( sig <= 0 )
				throw libc::exception( EDOM, "[gaussian_p::pdf] invalid sigma=%g", sig );
			
			if( x <= 0 )
				return 0;
			else
			{
				const   real_t X   = (x - mu) / sig;
				const   real_t num = Exp( - (X*X) * REAL(0.5) );
				const   real_t den = ( sqrt2pi * sig );
				const   real_t nrm = REAL(0.5) * ( REAL(1.0) + qerf( mu/(sqrt2 * sig) ) );
				return  num / ( den * nrm  );
			}
		}
		
		template<>
		real_t gaussian_p<real_t>:: cdf( real_t x, const real_t mu, const real_t sig )
		{
			static const real_t sqrt2   = Sqrt( REAL(2.0) );
			if( sig <= 0 )
				throw libc::exception( EDOM, "[gaussian_p::cdf] invalid sigma=%g", sig );
			
			if( x <= 0 )
				return 0;
			else
			{
				const real_t S = sqrt2 * sig;
				const real_t X = (x-mu)/S;
				const real_t Y = mu/S;
				const real_t Z = qerf( Y );
				const real_t num = ( qerf(X) + Z );
				const real_t den = ( REAL(1.0) + Z );
				return num / den;
			}
		}
		
		template <>
		real_t log_normal<real_t>::pdf( real_t x, const real_t mu, const real_t sig )
		{
			static const real_t sqrt2pi = Sqrt( numeric<real_t>::two_pi );
			
			if( sig <= 0 )
				throw libc::exception( EDOM, "[log_normal::pdf] invalid sigma=%g", sig );
			
			if( x <=0 )
				return 0;
			else
			{
				const  real_t X = ( std::log( x ) - mu )/sig;
				return std::exp( - (X*X) * REAL(0.5) ) / ( sqrt2pi * sig * x );
			}
			
		}
		
		template <>
		real_t log_normal<real_t>::cdf( real_t x, const real_t mu, const real_t sig )
		{
			static const real_t sqrt2   = Sqrt( REAL(2.0) );
			
			if( sig <= 0 )
				throw libc::exception( EDOM, "[log_normal::cdf] invalid sigma=%g", sig );
			
			if( x <=0 )
				return 0;
			else
			{
				return REAL(0.5) *  ( REAL(1.0) + qerf( ( Log(x)-mu) / ( sqrt2 * sig ) ) );
			}
		}
		
		//-- Weibull
		template <>
		real_t weibull<real_t>::pdf( real_t x, const real_t k, const real_t lam )
		{
			if( k <=0 || lam <= 0)
				throw libc::exception( EDOM, "[weibull:pdf] invalid k=%g,lam=%g", k, lam );
			
			if( x <=0 )
				return 0;
			else
			{
				const real_t X = x/lam;
				return (k/lam) * Pow( X, (k-REAL(1.0)) ) * Exp( - Pow( X, k ) );
			}
			
		}
		
		template <>
		real_t weibull<real_t>:: cdf( real_t x, const real_t k, const real_t lam )
		{
			if( k <=0 || lam <= 0)
				throw libc::exception( EDOM, "[weibull:pdf] invalid k=%g,lam=%g", k, lam );
			
			if( x <=0 )
				return 0;
			else
			{
				const real_t X = x/lam;
				return REAL(1.0) - Exp( - Pow(X,k) );
			}
			
		}
		
		//-- gamma_law pdf
		template <>
		real_t gamma_law<real_t>::pdf( real_t x, const real_t k, const real_t lam )
		{
			if( k <=0 || lam <= 0)
				throw libc::exception( EDOM, "[gamma:pdf] invalid k=%g,lam=%g", k, lam );
			
			if( x <= 0 )
				return 0;
			else
			{
				const real_t X = x/lam;
				return X * Exp( - (X+gamma_log(k)) ) / lam;
			}
		}
		
		//-- gamma_law cdf
		template <>
		real_t gamma_law<real_t>::cdf( real_t x, const real_t k, const real_t lam )
		{
			if( k <=0 || lam <= 0)
				throw libc::exception( EDOM, "[gamma:cdf] invalid k=%g,lam=%g", k, lam );
			if( x <= 0 )
				return 0;
			else
			{
				const real_t X = x/lam;
				return gamma_p( k, X );
			}
		}
		
		template <>
		real_t student<real_t>:: cdf( real_t nu, real_t t )
		{
			if(nu<=0)
				throw libc::exception( EDOM, "[student_::cdf] nu=0");
			const real_t a = REAL(0.5) * nu;
			const real_t b = REAL(0.5);
			const real_t x = real_t(nu) / ( real_t(nu) + t*t);
			return clamp<real_t>(0,REAL(1.0)-beta_i(a,b,x),1);
		}
		
		
		namespace {
			
			struct zstudent_t
			{
				real_t nu;
				real_t p;
				inline real_t get( real_t t ) const { return student<real_t>::cdf( nu, t ) - p; }
			};
		}
		
		template <>
		real_t student<real_t>::icdf( real_t nu, real_t p )
		{
			if(nu<=0)
				throw libc::exception( EDOM, "[student_icdf] nu=0");
			if(p>=1)
				throw libc::exception( EDOM, "[student_icdf] p>=1");
			
			if( p <= 0 )
				return 0;
			else
			{
				const zstudent_t zstudent_ = { nu, p };
				numeric<real_t>::function zstudent( & zstudent_, & zstudent_t::get );
				//const real_t coef[2] = { nu, p };
				//void        *args = (void *)coef;
				//assert( zstudent(0,args) <= 0 );
				assert( zstudent(0) <= 0 );
				real_t       thi  = 1;
				while( zstudent( thi ) <= 0 )
					thi += thi;
				
				const zfind<real_t> solve(max_of<real_t>( REAL(1.0e-5), numeric<real_t>::ftol ));
				return solve( zstudent, 0, thi );
			}
		}
		
		
		//----
		template <>
		real_t CHI2<real_t>::cdf( real_t nu, real_t t )
		{
			if(nu<=0)
				throw libc::exception( EDOM, "[CHI2::cdf] nu<=0");
			if(t<0)
				throw libc::exception( EDOM, "[CHI2::cdf] t<0");
			
			return gamma_p( REAL(0.5) * nu, REAL(0.5) * t );
		}
		
#if 0
		static
		real_t zchi2( real_t t, void *args )
		{
			assert(args);
			const real_t *coef = (real_t *)args;
			const real_t  nu   = coef[0];
			const real_t  p    = coef[1];
			return CHI2<real_t>::cdf( nu, t ) - p;
		}
#endif
		
		namespace {
			struct zchi2_t
			{
				real_t nu;
				real_t p;
				inline real_t get( real_t t ) const { return CHI2<real_t>::cdf( nu, t ) - p; }
			};
		}
		
		template <>
		real_t CHI2<real_t>:: icdf( real_t nu, real_t p )
		{
			if(nu<=0)
				throw libc::exception( EDOM, "[CHI2::icdf] nu<=0");
			if(p>=1)
				throw libc::exception( EDOM, "[CHI2::icdf] p>=1");
			
			if( p <= 0 )
				return 0;
			else
			{
				assert( p>0 );
				assert( p<1 );
				const zchi2_t zchi2_ = { nu, p };
				numeric<real_t>::function zchi2( & zchi2_, & zchi2_t::get ); 
				assert( zchi2(0) <= 0 );
				
				real_t       thi  = nu;
				while( zchi2( thi ) <= 0 )
					++thi;
				
				const zfind<real_t> solve(max_of<real_t>( REAL(1.0e-5), numeric<real_t>::ftol ));
				return solve( zchi2, 0, thi );
			}
		}
		
		
#if 0
		static
		real_t zchi2_delta( real_t delta_chi2, void *args )
		{
			assert( args );
			const real_t *cof = (const real_t *)args;
			const real_t  hnu = cof[0];
			const real_t  omp = cof[1];
			return omp - gamma_q( hnu, real_t(0.5) * delta_chi2 );
		}
#endif
		
		namespace 
		{
			struct zchi2_delta_t 
			{
				real_t hnu;
				real_t omp;
				inline real_t get(real_t delta_chi2 ) const
				{
					return omp - gamma_q( hnu, real_t(0.5) * delta_chi2 );
				}
			};
		}
		
		template <>
		real_t CHI2<real_t>:: delta( real_t nu, real_t p) {
			assert(nu>0);
			assert(p>=0);
			assert(p<1);
			
			//const real_t cof[2] = { REAL(0.5) * real_t(nu), REAL(1.0) - p };
			//void        *args   = (void *)cof;
			
			const zchi2_delta_t zchi2_delta_ =  { REAL(0.5) * real_t(nu), REAL(1.0) - p };
			numeric<real_t>::function zchi2_delta( & zchi2_delta_, & zchi2_delta_t :: get );
			assert( zchi2_delta(0) < 0 );
			real_t x = 2;
			while( zchi2_delta(x) <= 0 ) {
				x *= x;
			}
			const zfind<real_t> solve(max_of<real_t>( REAL(1.0e-5), numeric<real_t>::ftol ));
			return solve(zchi2_delta,0,x);
			
		}
		
		
	}
	
}
