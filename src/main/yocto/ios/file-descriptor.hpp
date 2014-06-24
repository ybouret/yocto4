#ifndef YOCTO_IOS_FILE_DESCRIPTOR_INCLUDED
#define YOCTO_IOS_FILE_DESCRIPTOR_INCLUDED 1


#include "yocto/error.hpp"



namespace yocto
{
	
	struct _fd
	{
		
#if defined(YOCTO_BSD)
		typedef int type;
#endif
		
#if defined(YOCTO_WIN)
		typedef void *type;
#endif
		
		static type       invalid() throw();
		static void       get( type handle, void       *data, size_t size, size_t &done );
		static void       put( type handle, const void *data, size_t size, size_t &done );
		static error_type close(type handle) throw();
	};
	
}

#endif
