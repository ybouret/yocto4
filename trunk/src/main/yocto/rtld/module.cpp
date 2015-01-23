#include "yocto/rtld/module.hpp"
#include "yocto/exceptions.hpp"

#if defined(YOCTO_BSD)
#include <dlfcn.h>
#endif

namespace yocto
{

    module:: module( dylib *user_dll ) throw() :
    dll(user_dll)
    {
        assert(0!=dll);
        dylib_incr(dll);
    }

    module:: module( const module &other ) throw() : dll( other.dll )
    {
        dylib_incr(dll);
    }

    module:: ~module() throw()
    {
        assert(dll);
        if(dylib_decr(dll)<=0)
        {
            dylib_free(dll);
        }
        dll = 0;
    }


    void * module:: query( const string &symbol ) const throw()
    {
        assert(dll);
        return dylib_addr(dll,symbol.c_str());
    }

    void * module:: query( const char *symbol ) const throw()
    {
        assert(dll);
        return dylib_addr(dll,symbol);
    }
    
    
}


