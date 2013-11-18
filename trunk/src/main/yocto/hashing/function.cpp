#include "yocto/hashing/function.hpp"
#include "yocto/memory/buffer.hpp"
#include "yocto/chars.hpp"
#include <cstring>

namespace yocto
{

	namespace hashing
	{

		function:: ~function() throw() {}

		function:: function( size_t L, size_t W ) throw() : length(L), window(W)
		{
		}


		void function:: fill( void *buffer, size_t buflen,	const void *output, size_t outlen ) throw()
		{
			assert( !(buffer==NULL && buflen > 0 ) );
			assert( output != NULL );
			assert( outlen > 0 );
			
			if( buflen <= outlen )
				memcpy( buffer, output, buflen );
			else
			{
				assert( buflen > outlen );
				uint8_t       *p = (uint8_t *)buffer;
				while( buflen > outlen )
				{
					memcpy(p,output,outlen);
					p      += outlen;
					buflen -= outlen;
				}
				assert(buflen<=outlen);
				memcpy(p,output,buflen);
			}
		}
		
		void function:: operator()( const void *buffer, size_t buflen ) throw()
		{
			run( buffer, buflen );
		}
		
		void function:: operator()( const memory::ro_buffer &buf ) throw()
		{
			run( buf.ro(), buf.length() );
		}
		
		void function:: operator()( const char *buf ) throw()
		{
			run( buf, length_of(buf) );
		}


        void function:: out( memory::rw_buffer &output ) throw()
        {
            get( output.rw(), output.length() );
        }

	}

}

