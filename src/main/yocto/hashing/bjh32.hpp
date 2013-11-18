#ifndef YOCTO_HASHING_BJH32_INCLUDED
#define YOCTO_HASHING_BJH32_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		
		class bjh32 : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(bjh32,4,4);
			
		private:
			uint32_t hash;
			YOCTO_DISABLE_COPY_AND_ASSIGN(bjh32);
		};
		
	}
}

#endif
