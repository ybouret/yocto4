#include "yocto/hashing/function.hpp"

namespace yocto
{

	namespace hashing
	{

		function:: ~function() throw() {}

		function:: function( size_t L, size_t W ) throw() : length(L), window(W)
		{
		}


		void function:: fill( void *buffer, size_t buflen,	const void *output, size_t outlen )
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

	}

}