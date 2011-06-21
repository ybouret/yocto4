#include "yocto/random/default.hpp"

namespace yocto
{
	
	namespace Random
	{
		
		Default:: Default( Bits &bits ) throw() : gen_( bits.full<uint32_t>() )
		{
		}
		
		Default:: ~Default() throw()
		{
			
		}
		
		void Default:: seed( Bits &bits ) throw() 
		{
			gen_.reset( bits.full<uint32_t>() );
		}
		
		double Default:: operator()( void ) throw()
		{
			return gen_.get<double>();
		}
		
		
	}
	
}

