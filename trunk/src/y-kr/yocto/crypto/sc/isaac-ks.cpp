#include "yocto/crypto/sc/isaac-ks.hpp"

namespace yocto
{
	
	namespace crypto
	{
		
		isaac_key_stream:: ~isaac_key_stream() throw()
		{
		}
		
		isaac_key_stream:: isaac_key_stream( const memory::ro_buffer &key ) throw() :
		isaac_( key )
		{
		}
		
		void isaac_key_stream:: schedule( const memory::ro_buffer &key ) throw()
		{
			isaac_.reset( key );
		}
		
		uint8_t isaac_key_stream:: call() throw()
		{
			return uint8_t(isaac_.rand32());
		}

	}

}

