#include "yocto/crypto/sc/key-stream.hpp"
#include "yocto/code/unroll.hpp"

namespace yocto
{
	
	namespace crypto
	{
		
		key_stream:: ~key_stream() throw() {}
		key_stream::  key_stream() throw() {}
		
		
#define YKS_CIPHER1(INDEX) q[INDEX] ^= call()
		
		void key_stream:: cipher( void *buffer, size_t buflen ) throw()
		{
			assert(!(buflen>0&&buffer==NULL));
			uint8_t *q = static_cast<uint8_t*>(buffer);
			YOCTO_LOOP_FUNC(buflen,YKS_CIPHER1,0);
		}
		
		void key_stream:: cipher( memory::rw_buffer &buffer ) throw() { cipher( buffer.rw(), buffer.length() ); }
		
	}
}