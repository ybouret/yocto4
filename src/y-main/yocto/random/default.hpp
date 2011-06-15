#ifndef YOCTO_RANDOM_DEFAULT_INCLUDED
#define YOCTO_RANDOM_DEFAULT_INCLUDED 1

#include "yocto/code/rand.hpp"
#include "yocto/random/uniform.hpp"

namespace yocto {
	
	namespace Random 
	{
		
		class Default : public Uniform
		{
		public:
			explicit Default( Bits & ) throw();
			virtual ~Default() throw();
			
			virtual void seed( Bits & ) throw();
			virtual double operator()(void) throw() = 0;
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(Default);
			urand gen_;
		};
		
	}
	
}

#endif
