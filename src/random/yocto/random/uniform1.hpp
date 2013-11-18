#ifndef YOCTO_RANDOM1_INCLUDED
#define YOCTO_RANDOM1_INCLUDED 1

#include "yocto/random/uniform.hpp"

namespace yocto {
	
	namespace Random {
		
		class Uniform1 : public Uniform 
		{
		public:
			virtual ~Uniform1() throw();
			
			virtual double operator()(void) throw();
			virtual void   seed( Bits & s ) throw();
			
			explicit Uniform1( Bits & ) throw();
			
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(Uniform1);
			
			
			int32_t  seed_;
			int32_t  iy_;
			int32_t  iv_[32];
			
			void set( Bits &s ) throw();
			
			
		};
		
	}
	
}

#endif
