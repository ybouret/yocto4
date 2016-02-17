#include "yocto/mpl/natural.hpp"

namespace yocto {

    namespace mpl
    {

        size_t natural::bits() const throw()
        {
            YOCTO_CHECK_MPN(*this);
            if(size>0)
            {
                const size_t   imsb = size-1;
                const unsigned MSB  = byte[imsb]; assert(0!=MSB);
                const size_t   init = (imsb<<3);

                if(0!=(MSB&0x80)) return init+8;
                if(0!=(MSB&0x40)) return init+7;
                if(0!=(MSB&0x20)) return init+6;
                if(0!=(MSB&0x10)) return init+5;
                if(0!=(MSB&0x08)) return init+4;
                if(0!=(MSB&0x04)) return init+3;
                if(0!=(MSB&0x02)) return init+2;
                if(0!=(MSB&0x01)) return init+1;
            }
            return 0;
        }

    }

}

#include "yocto/random/bits.hpp"
#include "yocto/code/round.hpp"

#include <iostream>

namespace yocto
{

    namespace mpl
    {

        const uint8_t natural::_bit[8] =
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

        natural natural:: exp2(const size_t n)
        {
            const size_t nbits = n+1;
            const size_t bytes = YOCTO_ROUND8(nbits) >> 3;
            natural ans( bytes, as_capacity );
            ans.byte[ bytes-1 ] = _bit[ n & 7 ];
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

        natural natural::shr( const natural &lhs,  const size_t n )
        {
            if( n <= 0 )
                return lhs;
            else
            {
                const size_t l_bits = lhs.bits();
                if( n >= l_bits )
                {
                    return natural();
                }
                else
                {
                    assert( n > 0      );
                    assert( n < l_bits );
                    const size_t t_bits = l_bits - n;
                    const size_t t_size = YOCTO_ROUND8(t_bits) >> 3;

                    natural ans( t_size, as_capacity );
                    const  uint8_t *src = lhs.byte;
                    uint8_t        *dst = ans.byte;
                    size_t obit = t_bits;
                    size_t ibit = l_bits;
                    for( size_t i=t_bits; i>0; --i )
                    {
                        --ibit;
                        --obit;
                        if( src[ ibit >> 3 ] & _bit[ ibit & 7 ] )
                        {
                            dst[ obit >> 3 ] |= _bit[ obit & 7];
                        }

                    }

                    //ans.size = t_size;
                    assert( ans.byte[ ans.size - 1] != 0 );
                    YOCTO_CHECK_MPN(ans);
                    return ans;
                }
            }
        }


        natural natural::shl( const natural &lhs,  size_t n )
        {
            if( n <= 0 )
                return lhs;
            else
            {
                const size_t l_bits = lhs.bits();
                const size_t t_bits = l_bits + n;
                const size_t t_size = YOCTO_ROUND8(t_bits) >> 3;

                natural ans( t_size, as_capacity );
                const  uint8_t *src = lhs.byte;
                uint8_t        *dst = ans.byte;
                size_t obit = t_bits;
                size_t ibit = l_bits;
                for( size_t i=l_bits; i>0; --i )
                {
                    --ibit;
                    --obit;
                    if( src[ ibit >> 3 ] & _bit[ ibit & 7 ] )
                    {
                        dst[ obit >> 3 ] |= _bit[ obit & 7];
                    }

                }
                ans.rescan();
                return ans;
            }
        }


        natural natural::rand(size_t nbits)
        {
            if(nbits<=0) return  natural();
            Random::Bits &ran = Random::CryptoBits();
            natural ans=1;
            while(--nbits>0)
            {
                ans.shl().byte[0] |= static_cast<uint8_t>( ran() ? 1 : 0 );
            }
            return ans;
        }
    }
}


