#include "yocto/rtld/interface.hpp"
#include "yocto/exceptions.hpp"

namespace yocto
{

    rtld_content:: ~rtld_content() throw()
    {
        if(dll)
        {
            if(dylib_decr(dll)<=0)
            {
                dylib_free(dll);
            }
        }
        dll = 0;
    }
    
    rtld_content:: rtld_content(const string &soname, const char *api_name) :
    uid(api_name),
    msg( max_message_length ),
    dll( dylib_load(soname.c_str(), dylib_full, msg(), msg.length()))
    {
        if(dll)
        {
            dylib_incr(dll);
        }
    }

    bool rtld_content:: is_loaded() const throw()
    {
        return 0!=dll;
    }

#define yld_xstr(s) yld__str(s)
#define yld__str(s) #s

    const char rtld_content:: loader_name[] = yld_xstr(YOCTO_RTLD_LOADER);


    
}

