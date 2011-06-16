#ifndef YOCTO_KRYPTO_ISAAC_INCLUDED
#define YOCTO_KRYPTO_ISAAC_INCLUDED 1

#include "yocto/crypto/sc/key-stream.hpp"
#include "yocto/random/isaac.hpp"

namespace yocto
{
	
	namespace crypto
	{
		
		class isaac_key_stream : public key_stream
		{
		public:
			virtual ~isaac_key_stream() throw();
			explicit isaac_key_stream( const memory::ro_buffer & ) throw();
			
			virtual void      schedule( const memory::ro_buffer & ) throw();
			virtual uint8_t   call() throw();
			
		private:
			YOCTO_DISABLE_COPY_AND_ASSIGN(isaac_key_stream);
			Random::ISAAC isaac_;
		};
		
	}
	
}

#endif
