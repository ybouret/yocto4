#ifndef YOCTO_HASHING_SHA512_INCLUDED
#define YOCTO_HASHING_SHA512_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		struct SHA512_CTX
		{
			uint64_t total[2];          /*!< number of bytes processed  */
			uint64_t state[8];          /*!< intermediate digest state  */
			uint8_t  buffer[128];  		/*!< data block being processed */
			int      is384;             /*!< false => SHA-512, else SHA-384 */
		};
		
		
		class sha512 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(sha512,64,128);
			
		private:
			SHA512_CTX ctx;
			YOCTO_DISABLE_COPY_AND_ASSIGN(sha512);
		};
		
		class sha384 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(sha384,48,128);
			
		private:
			SHA512_CTX ctx;
			YOCTO_DISABLE_COPY_AND_ASSIGN(sha384);
		};
		
	}
	
}

#endif

	
	
