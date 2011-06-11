#ifndef YOCTO_HASHING_FNV_INCLUDED
#define YOCTO_HASHING_FNV_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		
		class fnv : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(fnv,4,4);
			
		private:
			uint32_t hash;
			YOCTO_DISABLE_COPY_AND_ASSIGN(fnv);
		};
		
	}
}

#endif
