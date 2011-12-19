#ifndef YOCTO_CODE_CAST_INCLUDED
#define YOCTO_CODE_CAST_INCLUDED


#include "yocto/os.hpp"


namespace yocto
{
	
	template <typename T>
	struct _cast
	{
		
		static inline void *load( T *item )    throw() { return (void*)item; }
		static inline T    *from( void *addr ) throw() { return (T*)addr; }
		
	};
}

#endif
