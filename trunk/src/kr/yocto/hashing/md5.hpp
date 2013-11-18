#ifndef YOCTO_HASHING_MD5_INCLUDED
#define YOCTO_HASHING_MD5_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		/* RFC 1321 */
		namespace RFC1321
		{
			struct MD5_CTX
			{
				uint32_t      state[4];        /*!< state (ABCD) */
				uint32_t      count[2];        /*!< number of bits, modulo 2^64 (lsb first) */
				uint8_t       buffer[64];      /*!< input buffer */		
			};
		}

		class md5 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(md5,16,64);

		private:
			RFC1321::MD5_CTX ctx;
			YOCTO_DISABLE_COPY_AND_ASSIGN(md5);
		};

	}
}

#endif
