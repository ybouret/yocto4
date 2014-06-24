#include "yocto/ios/ochannel.hpp"
#include "yocto/memory/buffer.hpp"

namespace yocto
{
	namespace ios
	{
		
		
		ochannel:: ~ochannel() throw()
		{
			
		}
		
		ochannel:: ochannel() throw()
		{
			
		}
		
		void ochannel:: put_all( const void *data, size_t size, size_t &done )
		{
			assert( !(data==NULL&&size>0 ) );
			const uint8_t *p       = (const uint8_t *)data;
			size_t         towrite = size;
			done = 0;
            size_t written = 0;
			try {
				while( towrite > 0 )
				{
					written  = put( p, towrite );
					done    += written;
					p       += written;
					towrite -= written;
				}
			}
			catch (...) {
				done    += written;
				throw;
			}
		}
		
		
		void ochannel:: put_all( const memory::ro_buffer &buff, size_t &done )
		{
			put_all( buff.ro(), buff.length(), done );
		}
		
	}
	
}

