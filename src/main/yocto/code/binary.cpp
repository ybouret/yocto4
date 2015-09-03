#include "yocto/code/binary.hpp"

namespace yocto
{
    size_t bytes_for( const size_t n) throw()
    {
        const size_t max_bytes = sizeof(size_t);
        size_t mask = static_cast<size_t>(0xff) << ( (max_bytes-1) * 8 );
        for( size_t i=max_bytes;i>0;--i)
        {
            if( n & mask )
                return i;
            mask >>= 8;
        }
        return 0;
    }

    size_t bits_for( const size_t n) throw()
    {
        const size_t max_bits = sizeof(size_t) * 8;
        size_t mask = static_cast<size_t>(0x01) << ( max_bits-1 );
        for( int i=max_bits;i>0;--i)
        {
            if( n & mask )
                return i;
            mask >>= 1;
        }
        return 0;
    }

    const uint8_t ceil_ln2_table[256] =
    {
        0,1,2,2,3,3,3,3,4,4,4,4,4,4,4,4,
        5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,5,
        6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
        6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,7,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,
        8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8
    };

    unsigned unsafe_getbit(const size_t i,const void *data) throw()
    {
        assert(data);
        const uint8_t *p = (const uint8_t *)data;
        p += (i>>3);
        if( 0 != (p[0] & (1<<(i&7)) ) )
            return 1;
        else
            return 0;
    }
    
}
