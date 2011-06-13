
#ifndef YOCTO_HASHING_SHA256_INCLUDED
#define YOCTO_HASHING_SHA256_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		
		
		
		typedef struct 
		{
			uint32_t      total[2];     /*!< number of bytes processed  */
			uint32_t      state[8];     /*!< intermediate digest state  */
			uint8_t       buffer[64];   /*!< data block being processed */
			int           is224;        /*!< false => SHA-256, else SHA-224 */
		} SHA256_CTX;
		
		
		class sha256 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(sha256,32,64);
			
		private:
			SHA256_CTX ctx;
			YOCTO_DISABLE_COPY_AND_ASSIGN(sha256);
		};
		
		class sha224 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(sha224,28,64);
			
		private:
			SHA256_CTX ctx;
			YOCTO_DISABLE_COPY_AND_ASSIGN(sha224);
		};
		
		
		
	}
}

#endif


