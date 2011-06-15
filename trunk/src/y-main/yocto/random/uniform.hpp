#ifndef YOCTO_RANDOM_INCLUDED
#define YOCTO_RANDOM_INCLUDED 1

#include "yocto/random/bits.hpp"

namespace yocto {
	
	namespace Random 
	{
				
		class Uniform : public object
		{
		public:
			virtual ~Uniform() throw();
			
			virtual double operator()(void)     throw() = 0;
			virtual void   seed( Bits &   )     throw() = 0;
			
			
		protected:
			explicit Uniform() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(Uniform);
		};
		
		
	}
	
}

#endif
