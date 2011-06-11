#ifndef YOCTO_HASHING_CRC16_INCLUDED
#define YOCTO_HASHING_CRC16_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		class crc16 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(crc16,2,2);
			
		private:
			uint16_t    hash;
			YOCTO_DISABLE_COPY_AND_ASSIGN(crc16);
		};
		
	}
}

#endif
