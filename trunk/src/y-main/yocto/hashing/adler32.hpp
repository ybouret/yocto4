#ifndef YOCTO_HASHING_ADLER32_INCLUDED
#define YOCTO_HASHING_ADLER32_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		
		class adler32 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(adler32,4,4);
			
		private:
			uint32_t a;
			YOCTO_DISABLE_COPY_AND_ASSIGN(adler32);
		};
		
	}
}

#endif
