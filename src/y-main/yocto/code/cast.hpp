#ifndef YOCTO_CODE_CAST_INCLUDED
#define YOCTO_CODE_CAST_INCLUDED


#include "yocto/os.hpp"


namespace yocto
{
	
	class _cast
	{
	public:
		template <typename T>
		static inline void *load( T *item )    throw() { return addr2addr(item); }
		
		template <typename T>
		static inline T    *from( void *addr ) throw() { return (T*)addr; }
	
		template <typename T, typename U>
		static inline T *trans( U *p ) throw() { return (T *)addr2addr(p); }
	
	private:
		static void * addr2addr( void * ) throw();
	};
}

#endif
