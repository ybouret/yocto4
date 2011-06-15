#ifndef YOCTO_ARC4_INCLUDED
#define YOCTO_ARC4_INCLUDED 1

#include "yocto/crypto/sc/key-stream.hpp"

namespace yocto
{
	
	namespace crypto
	{
		class arc4 : public key_stream
		{
		public:
			explicit arc4( const memory::ro_buffer &key ) throw();
			virtual ~arc4() throw();
			
			virtual void     schedule( const memory::ro_buffer &key ) throw();
			virtual uint8_t  call() throw();
			
		private:
			unsigned x;
			unsigned y;
			uint8_t  S[256];
			void    init( const uint8_t *key, size_t len ) throw();
		};
		
		
	}
}

#endif
