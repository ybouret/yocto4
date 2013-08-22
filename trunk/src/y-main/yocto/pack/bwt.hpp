#ifndef YOCTO_PACK_BWT_INCLUDED
#define YOCTO_PACK_BWT_INCLUDED 1

#include "yocto/pack/mtf.hpp"


namespace yocto
{
	
	namespace pack
	{
		
		struct bwt
		{
			static size_t encode( void *output, const void *input, const size_t size, size_t *indices, move_to_front *mtf = NULL) throw();
			static void   decode( void *output, const void *input, const size_t size, size_t *indices, size_t primary_index, move_to_front *mtf = NULL ) throw();
		};
		
	}
	
}


#endif
