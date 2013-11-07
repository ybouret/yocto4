#include "yocto/threading/singleton.hpp"
#include <iostream>
#include <cstdio>

namespace yocto
{
	bool singleton_verbosity = false;

    namespace hidden
    {
        void singleton_out(const char *name,
                           const char *mesg,
                           const int   life_time ) throw()
        {
            assert(name);
            assert(mesg);
            try
            {
                std::cerr.flush();
                fprintf( stderr, "[[ %20s $%-12d ]] <%s>\n", name, life_time, mesg);
                fflush(stderr);
            }
            catch(...)
            {
            }
        }
        
    }
}

