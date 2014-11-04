#ifndef YOCTO_FAME_TYPES_IO_INCLUDED
#define YOCTO_FAME_TYPES_IO_INCLUDED 1

#include "yocto/code/endian.hpp"
#include "yocto/type/args.hpp"
#include <cstring>

namespace yocto
{
    namespace fame
    {
        
        template <typename T>
        struct type_io
        {
            static inline void read(void * &dst, const void * &src )
            {
                T w(0);
                memcpy(&w,src, sizeof(T)); src = static_cast<const uint8_t*>(src) + sizeof(T);
                w = swap_be_as(w);
                memcpy(dst,&w,sizeof(T));  dst = static_cast<uint8_t*>(dst) + sizeof(T);
            }
        };
        
        
        
    }
}

#endif
