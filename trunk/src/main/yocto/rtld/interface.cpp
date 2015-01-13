#include "yocto/rtld/interface.hpp"
#include "yocto/exceptions.hpp"

namespace yocto
{

    rtld_content:: ~rtld_content() throw()
    {
    }


    static inline
    dylib *load_module(const char *soname )
    {
        char errbuf[256];
        const size_t errlen = sizeof(errbuf);
        memset(errbuf,0,errlen);
        dylib *so = dylib_load(soname, dylib_full, errbuf, errlen);
        if(!so)
        {
            throw imported::exception("dylib_load failure","%s",errbuf);
        }
        return so;
    }

    rtld_content:: rtld_content(const string &soname, const char *api_name) :
    uid(api_name),
    dll( load_module(soname.c_str()) )
    {
    }

#define yld_xstr(s) yld__str(s)
#define yld__str(s) #s

    const char rtld_content:: loader_name[] = yld_xstr(YOCTO_RTLD_LOADER);
    
    
}