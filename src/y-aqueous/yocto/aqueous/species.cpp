#include "yocto/aqueous/species.hpp"
#include "yocto/memory/pooled.hpp"

namespace yocto
{
    namespace aqueous
    {
        species:: ~species() throw()
        {
            if( addr )
            {
                if(kill)
                    kill(addr);
                memory::kind<memory::pooled>::release(addr,blen);
            }
        }
        
        species:: species(const string &id,
                          const int    charge,
                          size_t       extra_bytes
                          ) :
        name(id),
        z(charge),
        blen(extra_bytes),
        addr( memory::kind<memory::pooled>::acquire(blen) ),
        kill(NULL)
        {
        }
        
        species:: species(const char  *id,
                          const int    charge,
                          size_t       extra_bytes
                          ) :
        name(id),
        z(charge),
        blen(extra_bytes),
        addr( memory::kind<memory::pooled>::acquire(blen) ),
        kill(NULL)
        {
        }
        
        const string & species:: key() const throw() { return name; }
        
        
        
    }
}

