#ifndef YOCTO_RTLD_DYLIB_INCLUDED
#define YOCTO_RTLD_DYLIB_INCLUDED 1

#include "yocto/string.hpp"

namespace yocto
{
    
    struct  dylib;
    
    //! not thread safe for error message
    dylib      *dylib_load( const char *soname, char *errbuf=0, size_t errlen=0);
    
    
    //! increase refcount
    void        dylib_incr(dylib *dll) throw();
    
    
    //! decrease refcount and close if needed
    void        dylib_free(dylib *dll) throw();
    
    
}

#endif
