#ifndef YOCTO_HASHING_MD4_INCLUDED
#define YOCTO_HASHING_MD4_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		namespace RFC1320
		{
			struct MD4_CTX
			{
				uint32_t      state[4];        /*!< state (ABCD) */
				uint32_t      count[2];        /*!< number of bits, modulo 2^64 (lsb first) */
				uint8_t       buffer[64];      /*!< input buffer */
			} ;
			
		}
		class md4 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(md4,16,64);
			
		private:
			RFC1320::MD4_CTX ctx;
			YOCTO_DISABLE_COPY_AND_ASSIGN(md4);
		};
		
	}
}

#endif
