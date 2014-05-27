#ifndef _YOCTO_FUNCTIONS_HPP_
#define _YOCTO_FUNCTIONS_HPP_ 1

#include "yocto/math/types.hpp"

namespace yocto {

	namespace math {

		float  qerfc( float  X ) throw();
		double qerfc( double X ) throw();

		float  qerf( float  X ) throw();
		double qerf( double X ) throw();

		float  iqerf( float  p ) throw();//!< X|qerf(X)=p, -1<p<1
		double iqerf( double p ) throw();
       
        float  iqerfc( float  p ) throw(); //!< X|qerfc(X)=p, 0<p<2
        double iqerfc( double p ) throw(); 
        
		float  gamma_log( float  X ) throw();
		double gamma_log( double X ) throw();

		float  gamma_i( float  a, float  x ); //!< incomplete gamma
		double gamma_i( double a, double x ); //!< incomplete gamma

		float  gamma_p( float  a, float  x ); //!< P(a,x)
		double gamma_p( double a, double x ); //!< P(a,x)

		float  gamma_q( float  a, float  x ); //!< Q(a,x)
		double gamma_q( double a, double x ); //!< Q(a,x)

		float  beta_i( float  a, float  b, float  x );//! incomlete beta function
		double beta_i( double a, double b, double x );//! incomlete beta function
        
        
	}

}


#endif
