#ifndef YOCTO_NET_TYPES_INCLUDED
#define YOCTO_NET_TYPES_INCLUDED 1

#include "yocto/code/endian.hpp"
#include <iosfwd>

namespace yocto
{
    namespace network
    {

        typedef uint8_t  net8_t;
        typedef uint16_t net16_t;
        typedef uint32_t net32_t;
        typedef uint64_t net64_t;

        class net128_t
        {
        public:
            net128_t() throw();
            ~net128_t() throw();
            net128_t( const net128_t & ) throw();
            net128_t &operator=( const net128_t & ) throw();

            net128_t( const uint8_t buf[16] ) throw();

            uint8_t h[16];
            friend std::ostream & operator<< ( std::ostream &, const net128_t & );
            
        };
        
        inline uint16_t swap_nbo(const uint16_t x) throw()
        {
            return YOCTO_SWAP_BE16(x);
        }
        
        inline uint32_t swap_nbo(const uint32_t x) throw()
        {
            return YOCTO_SWAP_BE32(x);
        }

        
        inline uint64_t swap_nbo(const uint64_t x) throw()
        {
            return YOCTO_SWAP_BE64(x);
        }
        
    }
}

#endif
