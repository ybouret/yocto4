#ifndef YOCTO_HASHING_SFH_INCLUDED
#define YOCTO_HASHING_SFH_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		
		class sfh : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(sfh,4,4);
			
		private:
			uint32_t hash;
			YOCTO_DISABLE_COPY_AND_ASSIGN(sfh);
		};
		
	}
}

#endif
