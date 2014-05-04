#include "yocto/random/bivariate-gaussian.hpp"
#include "yocto/code/utils.hpp"

#if defined(_MSC_VER)
#	if !defined(_USE_MATH_DEFINES)
#		define  _USE_MATH_DEFINES 
#	endif
#endif

#include <cmath>

namespace yocto 
{
	
	namespace Random
	{
		BivariateGaussian:: BivariateGaussian( Uniform &ran, const double s1, const double s2, const double c12  ) throw() :
		sig1( max_of<double>(s1,0) ),
		sig2( max_of<double>(s2,0) ),
		corr( clamp<double>(0, c12, 1 ) ),
		xorr( sqrt( 1.0 - (corr*corr) ) ),
		ugen_( ran )
		{
		}
		
		BivariateGaussian:: ~BivariateGaussian() throw()
		{
		}
		
		
		void BivariateGaussian:: operator()( double &u1, double &u2 ) throw()
		{
			static const double two_pi = M_PI * 2;
			double         x1    = 0;
			while( x1<=0 ) x1    = ugen_();
			const double   x2    = ugen_();
			const double   l     = log( x1 );
			const double   theta = two_pi * x2;
			const double   rho   = sqrt( -(l+l) );
			
			const double z1 = rho * sin( theta );
			const double z2 = rho * cos( theta );
			
			u1 = sig1 * z1;
			u2 = sig2 * ( corr * z1 + xorr * z2 );
		}
		
		
	}
}

