#ifndef YOCTO_HASHING_CRC32_INCLUDED
#define YOCTO_HASHING_CRC32_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		class crc32 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(crc32,4,4);
			
		private:
			uint32_t    hash;
			YOCTO_DISABLE_COPY_AND_ASSIGN(crc32);
		};
		
	}
}

#endif
