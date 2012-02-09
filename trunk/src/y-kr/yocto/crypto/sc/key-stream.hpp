#ifndef YOCTO_CRYPTO_KEY_STREAM_INCLUDED
#define YOCTO_CRYPTO_KEY_STREAM_INCLUDED 1

#include "yocto/memory/blender.hpp"

namespace yocto
{
	namespace crypto
	{
		class key_stream : public memory::blender
		{
		public:
			
			virtual ~key_stream() throw();
			
			virtual void      schedule( const memory::ro_buffer & ) throw() = 0;
			virtual uint8_t   call() throw() = 0;
			virtual void      scramble( void *buffer, size_t buflen ) throw();
			
		protected:
			explicit key_stream() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(key_stream);
		};
	}
}

#endif
