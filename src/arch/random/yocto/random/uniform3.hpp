#ifndef YOCTO_RANDOM3_INCLUDED
#define YOCTO_RANDOM3_INCLUDED 1

#include "yocto/random/uniform.hpp"

namespace yocto {
	
	namespace Random {
		
		class Uniform3 : public Uniform
		{
		public:
			virtual ~Uniform3() throw();
			
			virtual double operator()(void)       throw();
			virtual void   seed( Bits &s ) throw();
			
			explicit Uniform3( Bits &s ) throw();
			
			
		private:
			Uniform3( const Uniform3 & );
			Uniform3&operator=( const Uniform3 & );
			void set( Bits & s ) throw();
			
			int32_t  inext_;
			int32_t  inextp_;
			int32_t  ma_[56];
		};
		
	}
	
}

#endif
