#include "yocto/crypto/sc/key-stream.hpp"
#include "yocto/code/unroll.hpp"

namespace yocto
{
	
	namespace crypto
	{
		
		key_stream:: ~key_stream() throw() {}
		key_stream::  key_stream() throw() {}
		
#if defined(__ICC)
#pragma warning ( disable : 2259 )
#endif
#define YKS_CIPHER1(INDEX) q[INDEX] ^= call()
		
		void key_stream:: scramble( void *buffer, size_t buflen ) throw()
		{
			assert(!(buflen>0&&buffer==NULL));
			uint8_t *q = static_cast<uint8_t*>(buffer);
			YOCTO_LOOP_FUNC(buflen,YKS_CIPHER1,0);
		}
		
	}
}
