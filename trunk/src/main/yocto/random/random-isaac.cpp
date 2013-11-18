#include "yocto/random/uniform-isaac.hpp"
#include <new>

namespace yocto {
	
	namespace Random {
		
		isaac_fast:: isaac_fast( Bits &s ) throw() :
		mseed_( s.full<uint32_t>() ),
		isaac_( &mseed_, 1 )
		{
		}
		
		isaac_fast:: ~isaac_fast() throw()
		{
		}
		
		void isaac_fast:: seed( Bits &s ) throw()
		{
			mseed_ = s.full<uint32_t>();
			new (&isaac_) ISAAC_FAST( &mseed_, 1 );
		}
		
		double isaac_fast:: operator() (void) throw() {
			return ( 0.5 + double(isaac_.rand32() )) / ( 4294967296.0 );
		}
		
		
		isaac_crypto:: isaac_crypto( Bits & s ) throw() :
		mseed_( s.full<uint32_t>() ),
		isaac_( &mseed_, 1 )
		{
		}
		
		isaac_crypto:: ~isaac_crypto() throw()
		{
		}
		
		void isaac_crypto:: seed( Bits & s ) throw()
		{
			mseed_ = s.full<uint32_t>();
			new (&isaac_) ISAAC( &mseed_, 1 );
		}
		
		double isaac_crypto:: operator() (void) throw() 
		{
			return ( 0.5 + double(isaac_.rand32() )) / ( 4294967296.0 );
		}
	}
	
}
