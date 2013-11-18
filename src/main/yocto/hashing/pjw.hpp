#ifndef YOCTO_HASHING_PJW_INCLUDED
#define YOCTO_HASHING_PJW_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		
		class pjw : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(pjw,4,4);
			
		private:
			uint32_t hash;
			YOCTO_DISABLE_COPY_AND_ASSIGN(pjw);
		};
		
	}
}

#endif
