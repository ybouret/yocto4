#include "yocto/random/gaussian.hpp"

#if defined(_MSC_VER)
#	if !defined(_USE_MATH_DEFINES)
#		define  _USE_MATH_DEFINES 
#	endif
#endif


#include <cmath>

namespace yocto {
	
	namespace Random {
		
		Gaussian:: Gaussian( Uniform &ran ) throw() :
		ugen_( ran ),
		next_(false),
		gran_(0)
		{
		}
		
		Gaussian:: ~Gaussian() throw() {
			
		}
		
		double Gaussian:: operator()(void) throw()
		{
			//static const double two_pi = ( std::acos(-1.0) * 2 );
			static const double two_pi = M_PI * 2;
			if( next_ ) 
			{
				next_ = false;
				return gran_;
			}
			else {
				double        x1    = 0;
				while( x1<=0 ) x1  = ugen_();
				const double x2    = ugen_();
				const double l     = log( x1 );
				const double theta = two_pi * x2;
				const double rho   = sqrt( -(l+l) );
				
				next_ = true;
				gran_ = rho * sin( theta );
				return  rho * cos( theta );
			}
		}
		
		
	}
	
}
