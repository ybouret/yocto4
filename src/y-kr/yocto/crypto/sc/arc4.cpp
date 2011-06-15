#include "yocto/crypto/sc/arc4.hpp"
namespace yocto
{
	
	namespace crypto
	{
	
		
		arc4:: ~arc4() throw()
		{
			init(NULL,0);
		}
	
#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif
		arc4:: arc4( const memory::ro_buffer &key ) throw() :
			x(0),
			y(0),
			S( )
		{
			init( (const uint8_t *)key.ro(), key.length() );
		}
		
#include "arc4.cxx"
	
		void arc4:: schedule( const memory::ro_buffer &key ) throw()
		{
			init( (const uint8_t *)key.ro(), key.length() );
		}
		
		
		
		
	}
	
	
}
