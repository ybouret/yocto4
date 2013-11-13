#ifndef YOCTO_CODE_BZSET_INCLUDED
#define YOCTO_CODE_BZSET_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    namespace core
    {
        
        //!using portable 4 bytes word
        typedef uint32_t bzset_word;
        
        template <size_t NW,size_t NB>
        struct bzset;
        
        
        //! multiple
        template <size_t NW>
        struct bzset<NW, 0>
        {
            static inline void zero(void *addr) throw()
            {
                bzset_word *p = (bzset_word*)addr;
                for(size_t i=0;i<NW;++i) p[i] = 0;
            }
        };
        
        //! multiple+1
        template <size_t NW>
        struct bzset<NW, 1>
        {
            static inline void zero(void *addr) throw()
            {
                bzset_word *p = (bzset_word*)addr;
                for(size_t i=0;i<NW;++i) p[i] = 0;
                *(uint8_t *)(p+NW) = 0;
            }
        };
        
        //! multiple+2
        template <size_t NW>
        struct bzset<NW, 2>
        {
            static inline void zero(void *addr) throw()
            {
                bzset_word *p = (bzset_word*)addr;
                for(size_t i=0;i<NW;++i) p[i] = 0;
                *(uint16_t *)(p+NW) = 0;
            }
        };
        
        //! multiple+3
        template <size_t NW>
        struct bzset<NW, 3>
        {
            static inline void zero(void *addr) throw()
            {
                bzset_word *p = (bzset_word*)addr;
                for(size_t i=0;i<NW;++i) p[i] = 0;
                uint16_t *q = (uint16_t *)(p+NW);
                q[0] = 0;
                *(uint8_t *)&q[1] = 0;
            }
        };
        
        //! N=0
        template <>
        struct bzset<0,0>
        {
            static inline void zero(void*) throw() {}
        };
        
        //! N=1
        template <>
        struct bzset<0,1>
        {
            static inline void zero(void*addr) throw() { *(uint8_t*)addr = 0; }
        };
        
        //! N=2
        template <>
        struct bzset<0,2>
        {
            static inline void zero(void*addr) throw() { *(uint16_t*)addr = 0; }
        };
        
        //! N=3
        template <>
        struct bzset<0,3>
        {
            static inline void zero(void*addr) throw()
            {
                uint16_t *p = (uint16_t *)addr;
                p[0] = 0;
                *(uint8_t *)&p[1] = 0;
            }
        };
        
        
        template <size_t N>
        inline void bzfill( void *addr ) throw()
        {
            bzset<N/sizeof(bzset_word),N%sizeof(bzset_word)>::zero(addr);
        }
    }
    
    
    template <typename T>
    inline void bzset( T &c_struct ) throw()
    {
        core::bzset<
        sizeof(T)/sizeof(core::bzset_word),
        sizeof(T)%sizeof(core::bzset_word)>::zero( &c_struct );
    }
}

#endif
