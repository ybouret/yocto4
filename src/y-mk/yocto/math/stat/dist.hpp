#ifndef YOCTO_STAT_FN_INCLUDED
#define YOCTO_STAT_FN_INCLUDED 1

#include "yocto/math/types.hpp"

namespace yocto {
	
	namespace math {
		
		//! gaussian
		template <class T>
		struct gaussian
		{
			static T pdf( T x, const T mu, const T sig );
			static T cdf( T x, const T mu, const T sig );
		};
		
		
		//! gaussian positive
		template <class T>
		struct gaussian_p
		{
			static T pdf( T x, const T mu, const T sig );
			static T cdf( T x, const T mu, const T sig );
		};
		
		
		//! log-normal
		template <class T>
		struct log_normal
		{
			static T pdf( T x, const T mu, const T sig );
			static T cdf( T x, const T mu, const T sig );
		};
		
		
		//! Weibull
		template <class T>
		struct weibull
		{
			static T pdf( T x, const T k, const T lam );
			static T cdf( T x, const T k, const T lam );
		};
		
		
		//! gamma
		template <class T>
		struct gamma_law
		{
			static T pdf( T x, const T k, const T lam);
			static T cdf( T x, const T k, const T lam );
		};
		
		//! student
		template <class T>
		struct student
		{
			static T cdf( T nu, T t );  //!< student law of t with nu degrees of freedom
			static T icdf( T nu, T p ); //!< student::cdf(nu,t) = p (0<=p<1)
		};
		
		//! student
		template <class T>
		struct CHI2
		{
			static T cdf( T nu, T t );   //!< CHI2 law of t with nu degrees of freedom
			static T icdf(  T nu, T p ); //!< CHI2::cdf(nu,t) = p (0<=p<1)
			static T delta( T nu, T p ); //!< gamma_q(nu/2,delta_chi2/2)=1-p (0<=p<1)
		};
		
	}
	
}


#endif
