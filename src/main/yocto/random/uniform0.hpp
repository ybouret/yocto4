#ifndef YOCTO_RANDOM0_INCLUDED
#define YOCTO_RANDOM0_INCLUDED 1

#include "yocto/random/uniform.hpp"

namespace yocto {
	
	namespace Random  {
		
		class Uniform0 : public Uniform {
		public:
			virtual ~Uniform0() throw();
			
			virtual double operator()(void)       throw();
			virtual void   seed( Bits & ) throw();
			
			explicit Uniform0( Bits & ) throw();
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(Uniform0);
			int32_t   seed_;
			void set( Bits & ) throw();
			
		};
		
	}
	
}

#endif
