#ifndef YOCTO_ASSOC_SUB_HASHER_INCLUDED
#define YOCTO_ASSOC_SUB_HASHER_INCLUDED 1

#include "yocto/associative/key-hasher.hpp"

namespace yocto
{
    
    template <typename KEY,typename SUBKEY, typename HFUNC = hashing::sfh >
    class sub_hasher
    {
    public:
        static const size_t length = HFUNC:: __length;
        
        key_hasher<KEY,HFUNC>    h1;
        key_hasher<SUBKEY,HFUNC> h2;
        
        explicit sub_hasher() throw() : h1(), h2() {}
        virtual ~sub_hasher() throw() {}
        
    private:
        
        YOCTO_DISABLE_COPY_AND_ASSIGN(sub_hasher);
    };
    
}

#endif
