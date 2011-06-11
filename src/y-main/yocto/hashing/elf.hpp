#ifndef YOCTO_HASHING_ELF_INCLUDED
#define YOCTO_HASHING_ELF_INCLUDED 1

#include "yocto/hashing/function.hpp"

namespace yocto
{
	namespace hashing
	{
		
		class elf : public function
		{
		public:
			YOCTO_HASHING_FUNCTION_DECL(elf,4,4);
			
		private:
			uint32_t hash;
			YOCTO_DISABLE_COPY_AND_ASSIGN(elf);
		};
		
	}
}

#endif
