#ifndef YOCTO_CODE_BZSET_INCLUDED
#define YOCTO_CODE_BZSET_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    namespace core
    {
#define YOCTO_BZSET_USE_WORD
        
#if defined(YOCTO_BZSET_USE_WORD)
        template <size_t N> void __bzfill(void *addr) throw();
        template <>
        inline void __bzfill<0>(void*) throw() {}
        
        template <>
        inline void __bzfill<1>(void *addr) throw() { *(uint8_t*)addr=0; }
        
        template <>
        inline void __bzfill<2>(void *addr) throw() { *(uint16_t*)addr=0; }
        
        template <>
        inline void __bzfill<3>(void *addr) throw()
        {
            uint8_t *p = (uint8_t*)addr;
            p[0] = 0;
            p[1] = 0;
            p[2] = 0;
        }
        
        template <>
        inline void __bzfill<4>(void *addr) throw()
        {
            *(uint32_t *)addr = 0;
        }
        
        template <>
        inline void __bzfill<5>(void *addr) throw()
        {
            uint32_t *p = (uint32_t *)addr;
            *(p++) = 0;
            *(uint8_t*)p = 0;
        }
        
        
        template <>
        inline void __bzfill<6>(void *addr) throw()
        {
            uint32_t *p = (uint32_t *)addr;
            *(p++) = 0;
            *(uint16_t*)p = 0;
        }
        
        template <>
        inline void __bzfill<7>(void *addr) throw()
        {
            uint32_t *p = (uint32_t *)addr;
            *(p++) = 0;
            uint8_t *q = (uint8_t *)p;
            q[0] = 0;
            q[1] = 0;
            q[2] = 0;
        }

        
        template <size_t N>
        inline void bzset( void *addr ) throw()
        {
            typedef uint64_t     bzset_word;
            bzset_word *p = (bzset_word *)addr;
            for(size_t i=N/sizeof(bzset_word);i>0;--i)
                *(p++) = 0;
            __bzfill<N%sizeof(bzset_word)>(p);
        }
#else
        
        template <size_t N>
        inline void bzset( void *addr ) throw()
        {
            uint8_t *p = static_cast<uint8_t*>(addr);
            for(size_t i=0;i<N;++i) p[i] = 0;
        }
#endif
        
        template <>
        inline void bzset<1>(void *addr) throw()
        {
            *(uint8_t*)addr = 0;
        }
        
        template <>
        inline void bzset<2>(void *addr) throw()
        {
            *(uint16_t*)addr = 0;
        }
        
        template <>
        inline void bzset<3>(void *addr) throw()
        {
            uint16_t *p=(uint16_t *)addr;
            *(p++) = 0;
            *(uint8_t*)p = 0;
        }
        
        template <>
        inline void bzset<4>(void *addr) throw()
        {
            *(uint32_t*)addr = 0;
        }
        
        template <>
        inline void bzset<5>(void *addr) throw()
        {
            uint32_t *p = (uint32_t *)addr;
            *(p++) = 0;
            *(uint8_t *)p=0;
        }
        
        template <>
        inline void bzset<6>(void *addr) throw()
        {
            uint32_t *p = (uint32_t *)addr;
            *(p++) = 0;
            *(uint16_t *)p=0;
        }
        
        template <>
        inline void bzset<7>(void *addr) throw()
        {
            uint32_t *p = (uint32_t *)addr;
            *(p++) = 0;
            uint16_t *q = (uint16_t *)p;
            *(q++)=0;
            *(uint8_t *)q = 0;
        }
        
        
        template <>
        inline void bzset<8>(void *addr) throw()
        {
            *(uint64_t*)addr = 0;
        }
        
    }
    
    
    template <typename T>
    void bzset( T &c_struct ) throw()
    {
        core::bzset<sizeof(T)>( &c_struct );
    }
}

#endif
