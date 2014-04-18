#include "yocto/math/fcn/functions.hpp"

#include "yocto/math/ztype.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/exceptions.hpp"
#include "yocto/math/fcn/zfind.hpp"


#include <cerrno>
#include <iostream>

namespace yocto {
	
	namespace math {
		
		real_t qerfc( real_t x) throw()
        {
			const real_t z  = Fabs(x);
			const real_t t  = REAL(1.0)/(REAL(1.0)+REAL(0.5)*z);
			const real_t ans= t*Exp(-z*z-REAL(1.26551223)+t*(REAL(1.00002368)+t*(REAL(0.37409196)+t*(REAL(0.09678418)+
                                                                                                     t*(-REAL(0.18628806)+t*(REAL(0.27886807)+t*(-REAL(1.13520398)+t*(REAL(1.48851587)+
                                                                                                                                                                      t*(-REAL(0.82215223)+t*REAL(0.17087277))))))))));
			return x >= REAL(0.0) ? ans : REAL(2.0)-ans;
		}
		
		real_t qerf( real_t x) throw()
        {
			return REAL(1.0)-qerfc( x );
		}
		
		
		namespace
		{
			struct zqerf_type
			{
				real_t p;
				inline real_t compute_diff_erf( real_t x ) const throw()
				{
					return qerf(x)-p;
				}
                
                inline real_t compute_diff_erfc( real_t x ) const throw()
                {
                    return qerfc(x) - p;
                }
                
			};
		}
		
		real_t iqerf( real_t p ) throw()
        {
			assert(p>-1);
			assert(p<1);
			const zqerf_type          zqerf_args = { p };
			numeric<real_t>::function zqerf( &zqerf_args, & zqerf_type::compute_diff_erf );
			real_t x_lo = -1;
			while( zqerf(x_lo) >= 0)
            {
                x_lo += x_lo;
            }
            real_t x_hi = 1;
            while( zqerf(x_hi) <= 0)
            {
                x_hi += x_hi;
            }
            
            
            const zfind<real_t> solve( max_of<real_t>( REAL(1.0e-5), numeric<real_t>::ftol ) );
            return solve( zqerf, x_lo, x_hi );
        }
        
        real_t iqerfc( real_t p ) throw()
        {
			assert(p>0);
			assert(p<2);
			const zqerf_type          zqerfc_args = { p };
			numeric<real_t>::function zqerfc( &zqerfc_args, & zqerf_type::compute_diff_erfc );
			real_t x_lo = -1;
			while( zqerfc(x_lo) <= 0)
            {
                x_lo += x_lo;
            }
            
			real_t x_hi = 1;
			while( zqerfc(x_hi) >= 0)
            {
                x_hi += x_hi;
			}
			const zfind<real_t>        solve( max_of<real_t>( REAL(1.0e-5), numeric<real_t>::ftol ) );
			return solve( zqerfc, x_lo, x_hi );
		}
		
		real_t gamma_log( real_t xx ) throw() {
			static const double cof[6]=
			{
				+76.18009172947146,
				-86.50532032941677,
				+24.01409824083091,
				-01.231739572450155,
				-00.1208650973866179e-2,
				-00.5395239384953e-5
			};
			
			double x    = xx;
			double y    = xx;
			double tmp  = x+5.5;
			double ser  = 1.000000000190015;
			tmp -= (x+0.5)*std::log(tmp);
			ser += cof[0]/++y;
			ser += cof[1]/++y;
			ser += cof[2]/++y;
			ser += cof[3]/++y;
			ser += cof[4]/++y;
			ser += cof[5]/++y;
#if defined(__INTEL_COMPILER)
#pragma warning (push)
#pragma warning ( disable : 2259 )
#endif
			return static_cast<real_t>( log(2.5066282746310005*ser/x)-tmp);
#if defined(__INTEL_COMPILER)
#pragma warning (pop)
#endif
		}
		
		
		
		
		real_t gamma_i( real_t a, real_t x )
		{
			static const real_t EPS = numeric<real_t>::epsilon;
			//std::cerr << "gamma_i(" << a << "," << x << ")" << std::endl;
			
			if (x < REAL(0.0) || a <= REAL(0.0) )
				throw libc::exception(EDOM,"invalid (%g,%g) for gamma_i", a, x);
			
			real_t num = 1;
			real_t den = a;
			real_t ser = a;
			
			const real_t fac =  std::pow(x,a)*std::exp(-x);
			//std::cerr << "fac=" << fac << std::endl;
			//exit(1);
			real_t sum = num/den;
			real_t rho = sum;
			
			while( rho >= EPS ) {
				sum += ( rho = ( ( num *= x) / ( den *= ++ser ) ) );
			}
			
			real_t old_res = fac * sum;
			for(;;) {
				sum += ( ( num *= x) / ( den *= ++ser ) ) ;
				const real_t new_res = fac * sum;
				//std::cerr << "new_res=" << new_res << ", old_res=" << old_res << std::endl;
				if( (new_res - old_res) <= EPS * new_res )
					return new_res;
				old_res = new_res;
			}
		}
		
		real_t gamma_p( real_t a, real_t x ) {
			return clamp<real_t>(0,gamma_i(a,x) * Exp( -gamma_log(a) ),1);
		}
		
		
		real_t gamma_q( real_t a, real_t x ) {
			return clamp<real_t>(0,1-gamma_p(a,x),1);
		}
		
		static
		real_t betacf( const real_t a, const real_t b, real_t x )
		{
			static const real_t FPMIN = Pow( real_t(10.0), Ceil( Log10( real_t(REAL_MIN) ) ) );
			static const real_t EPS   = REAL_EPSILON;
			static const size_t MAXIT = 16384;
			
			//std::cout << "FPMIN=" << FPMIN << std::endl;
			//std::cout << "EPS  =" << EPS   << std::endl;
			
			const real_t qab = a+b;
			const real_t qap = a+REAL(1.0);
			const real_t qam = a-REAL(1.0);
			real_t c = REAL(1.0);
			real_t d = REAL(1.0) - qab *x/qap;
			if( Fabs(d) < FPMIN) d = FPMIN;
			d = REAL(1.0) / d;
			real_t h = d;
			for(size_t m=1;;)
			{
				const size_t m2 = m << 1;
				real_t aa = m * (b-m) * x / ((qam+m2)*(a+m2));
				d =  REAL(1.0)+aa*d;
				c =  REAL(1.0)+aa/c;
				if( Fabs(d) < FPMIN ) d = FPMIN;
				if( Fabs(c) < FPMIN ) c = FPMIN;
				d = REAL(1.0)/d;
				
				h *= (d*c);
				aa = -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));
				
				d =  REAL(1.0)+aa*d;
				c =  REAL(1.0)+aa/c;
				if( Fabs(d) < FPMIN ) d = FPMIN;
				if( Fabs(c) < FPMIN ) c = FPMIN;
				d = REAL(1.0)/d;
				const real_t del = d*c;
				h *= del;
				if( Fabs(del-REAL(1.0)) <= EPS )
					break;
				if( ++m > MAXIT )
					throw libc::exception( EDOM, "beta_i(%g,%g,%g) #%u doesn't converge",a,b,x,unsigned(MAXIT));
			}
			
			return h;
		}
		
		real_t beta_i( real_t a, real_t b, real_t x )
		{
			if( a <= 0)
				throw libc::exception( EDOM, "[beta_i]: a=%g", a );
			
			if( b <= 0)
				throw libc::exception( EDOM, "[beta_i]: b=%g", b );
			
			if( x < 0 || x > 1 )
				throw libc::exception( EDOM, "[beta_i]: x=%g", x );
			
			if( x <= 0 )
				return 0;
			else
				if( x >= 1 )
					return 1;
				else
				{
					const real_t bt = Exp( gamma_log(a+b) - gamma_log(a) - gamma_log(b) + a * Log(x) + b * Log( real_t(REAL(1.0)-x) ) );
					if( x < ( a+ REAL(1.0))/(a+b+REAL(2.0)) )
						return bt * betacf(a,b,x)/a;
					else
						return REAL(1.0) - bt * betacf(b,a,REAL(1.0)-x)/b;
				}
			
		}
        
        real_t bessel_I0( real_t x)
        {
            const double ax = fabs(x);
            if(ax<3.75)
            {
                double y=ax/3.75;
                y*=y;
                return 1.0+y*(3.5156229+y*(3.0899424+y*(1.2067492+y*(0.2659732+y*(0.360768e-1+y*0.45813e-2)))));
            }
            else
            {
                double y=3.75/ax;
                return (exp(ax)/sqrt(ax))*(0.39894228+y*(0.1328592e-1+y*(0.225319e-2+y*(-0.157565e-2+y*(0.916281e-2 +y*(-0.2057706e-1+y*(0.2635537e-1+y*(-0.1647633e-1 +y*0.392377e-2))))))));
            }
		}
        
        real_t bessel_K0( real_t x )
        {
            if (x <= 2.0)
            {
                const double y=x*x/4.0;
                return (-log(x/2.0)*bessel_I0(x))+(-0.57721566+y*(0.42278420 +y*(0.23069756+y*(0.3488590e-1+y*(0.262698e-2 +y*(0.10750e-3+y*0.74e-5))))));
            }
            else
            {
                const double y=2.0/x;
                return (exp(-x)/sqrt(x))*(1.25331414+y*(-0.7832358e-1 +y*(0.2189568e-1+y*(-0.1062446e-1+y*(0.587872e-2 +y*(-0.251540e-2+y*0.53208e-3))))));
            }
        }
		
	} //math
	
} //yocto
