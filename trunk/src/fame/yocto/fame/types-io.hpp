#ifndef YOCTO_FAME_TYPES_IO_INCLUDED
#define YOCTO_FAME_TYPES_IO_INCLUDED 1

#include "yocto/code/endian.hpp"
#include "yocto/math/v2d.hpp"
#include "yocto/math/v3d.hpp"

#include <cstring>

namespace yocto
{
    namespace fame
    {
        
        template <typename T>
        struct type_io
        {
            static inline void put(void * &dst,  const void *src )
            {
                T w(0);
                memcpy(&w,src, sizeof(T));
                w = swap_be_as(w);
                memcpy(dst,&w,sizeof(T));
                dst = static_cast<uint8_t*>(dst) + sizeof(T);
            }
        };
        
        template <>
        struct type_io< math::v2d<float> >
        {
            static inline void put(void * &dst, const void *src )
            {
                type_io<float>::put(dst,src);
                type_io<float>::put(dst,src);
            }
        };
        
        template <>
        struct type_io< math::v2d<double> >
        {
            static inline void put(void * &dst, const void *src )
            {
                type_io<double>::put(dst,src);
                type_io<double>::put(dst,src);
            }
        };
        
        template <>
        struct type_io< math::v3d<float> >
        {
            static inline void put(void * &dst, const void *src )
            {
                type_io<float>::put(dst,src);
                type_io<float>::put(dst,src);
                type_io<float>::put(dst,src);
            }
        };
        
        template <>
        struct type_io< math::v3d<double> >
        {
            static inline void put(void * &dst, const void *src )
            {
                type_io<double>::put(dst,src);
                type_io<double>::put(dst,src);
                type_io<double>::put(dst,src);
            }
        };


        
        
    }
}

#endif
