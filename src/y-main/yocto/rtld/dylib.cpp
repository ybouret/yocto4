#include "yocto/rtld/dylib.hpp"
#include "yocto/code/utils.hpp"

#if defined(YOCTO_BSD)
#include <dlfcn.h>
#endif

#include <cstring>

namespace yocto
{
    
    struct dylib
    {
        void  *h; //!< handle
        size_t n; //!< #ref
    };
    
    dylib *dylib_load( const char *soname, char *errbuf, size_t errlen)
    {
        
        assert(!(0==errbuf&&errlen>0));
        
        dylib *dll = object::acquire1<dylib>();
        memset(errbuf,0,errlen);
        YOCTO_GIANT_LOCK();
        
#if defined(YOCTO_BSD)
        dll->h = dlopen(soname, RTLD_NOW);
        if(0==dll->h)
        {
            if(errbuf)
            {
                assert(errlen>0);
                const char  *dl_err = dlerror();
                const size_t dl_len = length_of( dl_err );
                memcpy(errbuf,dl_err,min_of<size_t>(dl_len,errlen-1));
            }
            return 0;
        }
#endif
        
        dll->n = 1;
        return dll;
    }
    
    
    void dylib_free( dylib *dll ) throw()
    {
        assert(dll);
        assert(dll->n>0);
        assert(dll->h);
        if( --dll->n <= 0 )
        {
#if defined(YOCTO_BSD)
            dlclose(dll->h);
#endif
            
            object::release1<dylib>(dll);
        }
    }
    
}
