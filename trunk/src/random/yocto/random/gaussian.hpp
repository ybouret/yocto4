#ifndef YOCTO_RANDOM_GAUSSIAN_INCLUDED
#define YOCTO_RANDOM_GAUSSIAN_INCLUDED 1

#include "yocto/random/uniform.hpp"

namespace yocto {
	
	namespace Random {
		
		class Gaussian : public object
		{
		public:
			virtual ~Gaussian() throw();
			explicit Gaussian( Uniform &ran ) throw();
			
			double operator()(void)     throw();
			
		private:
			Gaussian( const Gaussian & );
			Gaussian &operator=( const Gaussian & );
			Uniform           &ugen_;
			bool               next_; //!< has next
			double             gran_; //!< next value
		};
		
	}
	
}

#endif
