#ifndef YOCTO_CODE_BWT_INCLUDED
#define YOCTO_CODE_BWT_INCLUDED 1

#include "yocto/os.hpp"


namespace yocto
{
	
	namespace core
	{
		
		struct bwt
		{
			static size_t encode( void *output, const void *input, const size_t size, size_t *indices ) throw();
			static void   decode( void *output, const void *input, const size_t size, size_t *indices, size_t primary_index ) throw();

		};
		
	}
	
}


#endif
