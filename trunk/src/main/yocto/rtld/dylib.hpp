#ifndef YOCTO_RTLD_DYLIB_INCLUDED
#define YOCTO_RTLD_DYLIB_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto
{
    
    //! abstract representation
    struct  dylib;
    
    enum dylib_flag
    {
        dylib_full,
        dylib_lazy
    };
    
    //! only memory exception is raised, may return NULL
    dylib      *dylib_load( const char *soname, dylib_flag flag, char *errbuf, size_t errlen);

    
	//! exception in case of error
    dylib      *dylib_load(const char *soname, dylib_flag flag);

    //! increase reference count
    void        dylib_incr(dylib *dll) throw();
    
	//! decrease and return reference count
	size_t      dylib_decr(dylib *dll) throw();
	
    //! decrease refcount and close if needed
    void        dylib_free(dylib *dll) throw();
    
	//! get symbol address
    void       *dylib_addr(const dylib *dll,const char *symbol) throw();
	
}

#endif
