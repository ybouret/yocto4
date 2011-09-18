#ifndef YOCTO_OBJECT_INCLUDED
#define YOCTO_OBJECT_INCLUDED 1

#include "yocto/memory/small-object.hpp"

#if !defined(YOCTO_LIMIT_SIZE)
#define YOCTO_LIMIT_SIZE 64
#endif

#if !defined(YOCTO_CHUNK_SIZE)
#define	YOCTO_CHUNK_SIZE 4096
#endif

namespace yocto
{
	typedef memory::small_object<YOCTO_LIMIT_SIZE,YOCTO_CHUNK_SIZE> object;	
}

#endif

