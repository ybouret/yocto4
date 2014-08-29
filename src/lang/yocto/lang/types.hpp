#ifndef YOCTO_LANG_TYPES_INCLUDED
#define YOCTO_LANG_TYPES_INCLUDED 1

#include "yocto/ordered/sorted-vector.hpp"

namespace yocto
{
    namespace lang
    {
        
        typedef uint8_t  code_type;
        typedef
        sorted_vector<
        code_type,
        comparator<code_type>,
        memory::global::allocator> bytes_store;
        
        
    }
}

#endif

