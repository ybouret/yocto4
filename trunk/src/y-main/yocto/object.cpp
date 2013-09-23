
#include "yocto/object.hpp"

namespace yocto
{
	namespace memory
	{
		template <>
		const char small_object<YOCTO_LIMIT_SIZE,YOCTO_CHUNK_SIZE>::provider::name[] = "object";
	}
    
    // object::  object() throw() {}
    // object:: ~object() throw() {}
	
}
