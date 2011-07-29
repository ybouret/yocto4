#ifndef _YOCTO_RANDOM_MT_HPP_
#define _YOCTO_RANDOM_MT_HPP_ 1

#include "yocto/random/uniform.hpp"

namespace yocto {
	
	namespace Random{
		
		class UniformMT : public Uniform 
		{
		public:
			virtual ~UniformMT() throw();
			
			virtual double operator()(void)       throw();
			virtual void   seed( Bits &s ) throw();
			
			explicit UniformMT( Bits &s ) throw();
			
			
		private:
			UniformMT( const UniformMT & );
			UniformMT&operator=( const UniformMT & );
			
			int           mti;
			unsigned long mt[624];
			
			void set( Bits &s ) throw();
			
		};
		
	}
	
}

#endif
