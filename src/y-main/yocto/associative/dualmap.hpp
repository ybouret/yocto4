#ifndef YOCTO_ASSOCIATIVE_DUAL_MAP_INCLUDED
#define YOCTO_ASSOCIATIVE_DUAL_MAP_INCLUDED 1

#include "yocto/type/sub-key.hpp"
#include "yocto/memory/global.hpp"
#include "yocto/hashing/sfh.hpp"

namespace yocto
{
    namespace hidden
    {
        extern const char dualmap_name[];
    }
    
    template <
    typename KEY,
    typename SUBKEY,
    typename T,
    typename HFUNC     = hashing::sfh,
    typename ALLOCATOR =  memory::global::allocator >
    class dualmap
    {
    public:
        YOCTO_ARGUMENTS_DECL_T;
        YOCTO_ARGUMENTS_DECL_KEY;
        YOCTO_ARGUMENTS_DECL_SUBKEY;
        
        
        virtual const char *name() const throw() { return hidden::dualmap_name; }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(dualmap);
    };
    
}

#endif

