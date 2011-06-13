#ifndef YOCTO_HASHING_SHA1_INCLUDED
#define YOCTO_HASHING_SHA1_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		
		namespace RFC3174
		{
			struct SHA1_CTX
			{
				uint32_t total[2];   /*!< number of bytes processed  */
				uint32_t state[5];   /*!< intermediate digest state  */
				uint8_t  buffer[64]; /*!< data block being processed */
			};
		}
		
		class sha1 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(sha1,20,64);
			
		private:
			RFC3174::SHA1_CTX ctx;
			YOCTO_DISABLE_COPY_AND_ASSIGN(sha1);
		};
		
	}
}

#endif
