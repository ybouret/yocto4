#ifndef YOCTO_PTR_REF_INCLUDED
#define YOCTO_PTR_REF_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    namespace core
    {
        size_t *create_ref();
        void    delete_ref( size_t * & ) throw();
    }

}

#endif
