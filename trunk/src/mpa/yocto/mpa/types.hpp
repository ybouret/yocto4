//! \file

#ifndef YOCTO_MPA_TYPES_INCLUDED
#define YOCTO_MPA_TYPES_INCLUDED 1

#include "yocto/os.hpp"

namespace yocto
{
    
    namespace mpa
    {
        class    memmgr;
        struct   memIO
        {
            static uint8_t *acquire(size_t &n);
            static void     release(uint8_t * &p, size_t &n) throw();
            static bool     random_bit();
        };
        
        //! helper
        template <typename T>
        class array_of
        {
        public:
            inline array_of( size_t n ) :
            bytes( n * sizeof(T) ),
            addr( (T *) memIO::acquire(bytes) ),
            size( bytes/sizeof(T) )
            {
                assert(size>=n);
            }
            
            inline ~array_of() throw()
            {
                uint8_t *p = (uint8_t *)addr;
                memIO::release(p, bytes);
            }
            
            inline T       & operator[](size_t indx) throw()        { assert(indx<size); return addr[indx]; }
            inline const T & operator[](size_t indx ) const throw() { assert(indx<size); return addr[indx]; }
            
        private:
            size_t bytes;
            T     *addr;
            YOCTO_DISABLE_COPY_AND_ASSIGN(array_of);
        public:
            const size_t size;
        };
        
    }
    
}

#endif
