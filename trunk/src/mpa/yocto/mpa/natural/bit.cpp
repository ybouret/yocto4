#include "yocto/mpa/natural.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/round.hpp"

namespace yocto
{
    namespace mpa
    {
        natural natural:: exp2(size_t n)
        {
            static const uint8_t  _bit[8] =
            {
                0x01,
                0x02,
                0x04,
                0x08,
                0x10,
                0x20,
                0x40,
                0x80
            };
            
            const size_t nbits = n+1;
			const size_t bytes = YOCTO_ROUND8(nbits) >> 3;
			natural ans( bytes, as_capacity );
			ans.byte[ bytes-1 ] = _bit[ n & 7 ];
            ans.size = bytes;
			return ans;
            
        }
        
        natural & natural:: shr() throw()
        {
            if( size > 0 )
            {
                uint8_t     *b = byte;
                const size_t n = size - 1;
                for( size_t i=0; i < n; )
                {
                    uint8_t &B = b[i];
                    B >>= 1;
                    B |= ( ( b[++i] & 0x1 ) << 7 ); //!< avoids a test !
                }
                b[n] >>= 1;
                update();
            }
            return *this;
        }
        
        
    }
    
}

