#ifndef YOCTO_RANDOM_BIVARIATE_GAUSSIAN_INCLUDED
#define YOCTO_RANDOM_BIVARIATE_GAUSSIAN_INCLUDED 1

#include "yocto/random/uniform.hpp"

namespace yocto 
{
	
	namespace Random
	{
		
		class BivariateGaussian : public object 
			{
			public:
				const double sig1; //!< sig1^2 = variance of u1
				const double sig2; //!< sig2^2 = variance of u2
				const double corr; //!< corr = var( u1.u2 ) / (sig1*sig2);
				const double xorr; //!< sqrt( 1 - corr^2 )
				
				virtual ~BivariateGaussian( ) throw();
				explicit BivariateGaussian( Uniform &ran, const double s1, const double s2, const double c12  ) throw();
				
				void operator()( double &u1, double &u2 ) throw();
				
			private:
				Uniform  &ugen_;
				YOCTO_DISABLE_COPY_AND_ASSIGN(BivariateGaussian);
			};
		
	}
	
}
#endif
