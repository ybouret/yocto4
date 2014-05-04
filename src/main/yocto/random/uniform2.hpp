#ifndef YOCTO_RANDOM2_INCLUDED
#define YOCTO_RANDOM2_INCLUDED 1

#include "yocto/random/uniform.hpp"

namespace yocto {
	
	namespace Random {
		
		class Uniform2 : public Uniform
		{
		public:
			virtual ~Uniform2() throw();
			
			virtual double operator()(void) throw();
			virtual void   seed( Bits & s ) throw();
			
			explicit Uniform2( Bits & ) throw();
			
			
		private:
			Uniform2( const Uniform2 & );
			Uniform2&operator=( const Uniform2 & );
			
			
			int32_t  seed_;
			int32_t  saux_;
			int32_t  iy_;
			int32_t  iv_[32];
			
			void set( Bits & s ) throw();
			
			
		};
		
	}
	
}

#endif
