#ifndef YOCTO_BSWAP_INCLUDED
#define YOCTO_BSWAP_INCLUDED 1

#include "yocto/os.hpp"


namespace yocto
{
    namespace core
    {
#define YOCTO_BSWAP_IMPL(TYPE) \
const TYPE tmp = *(TYPE *)lhs; \
*(TYPE *)lhs   = *(TYPE *)rhs; \
*(TYPE *)rhs   = tmp
        
        template <size_t N>
        inline void bswap( void *lhs, void *rhs ) throw();
        
        
        template <>
        inline void bswap<1>(void *lhs, void *rhs) throw()
        {
            YOCTO_BSWAP_IMPL(uint8_t);
        }
        
        template <>
        inline void bswap<2>(void *lhs, void *rhs) throw()
        {
            YOCTO_BSWAP_IMPL(uint16_t);
        }
        
        template <>
        inline void bswap<4>(void *lhs, void *rhs) throw()
        {
            YOCTO_BSWAP_IMPL(uint32_t);
        }
        
        template <>
        inline void bswap<8>(void *lhs, void *rhs) throw()
        {
            YOCTO_BSWAP_IMPL(uint64_t);
        }
        
        template <>
        inline void bswap<16>(void *lhs, void *rhs) throw()
        {
            uint64_t *L = (uint64_t *)lhs;
            uint64_t *R = (uint64_t *)rhs;
            {
                const uint64_t tmp = L[0];
                L[0] = R[0];
                R[0] = tmp;
            }
            
            {
                const uint64_t tmp = L[1];
                L[1] = R[1];
                R[1] = tmp;
            }
        }
        
    }
 
    template <typename T>
    inline void bswap(T &a, T &b) throw()
    {
        core::bswap<sizeof(T)>( &a, &b );
    }
    
}


#endif

