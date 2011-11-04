#include "yocto/hashing/hash-mac.hpp"
#include <cstring>

namespace yocto
{
	
	namespace hashing
	{
		
		hash_mac:: ~hash_mac() throw()
		{
		}
		
		hash_mac:: hash_mac( function &H, const memory::ro_buffer &key ) :
		L( H.length ),
		B( H.window ),
		key_( B, 0 ),
		tmp_( B, 0 ),
		end_( L, 0 )
		{
			assert( B == key_.length() );
			assert( B == tmp_.length() );
			assert( L == end_.length() );
			
			if( key.length() <= B )
			{
				memcpy( key_.rw(), key.ro(), key.length() );
			}
			else
			{
				H.set();
				H(key);
				H.get( key_.rw(), L );
			}
		}
		
		void hash_mac:: set( function &H ) throw()
		{
			assert( L == H.length );
			assert( B == H.window );
			H.set();
			for( size_t i=0; i < B; ++i ) tmp_[i] = uint8_t(key_[i] ^ 0x36);
			H(tmp_);
		}
		
		void hash_mac:: get( function &H, void *output, size_t outlen ) throw()
		{
			assert( L == H.length );
			assert( B == H.window );
			H.get( end_.rw(), L );
			H.set();
			for( size_t i=0; i < B; ++i ) tmp_[i] = uint8_t(key_[i] ^ 0x5c);
			H( tmp_ );
			H( end_ );
			H.get(output,outlen);
		}
		
	}
}
