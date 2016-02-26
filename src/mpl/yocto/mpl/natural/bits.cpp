#include "yocto/mpl/natural.hpp"
#include "yocto/code/round.hpp"

#include <iostream>

namespace yocto {

    namespace mpl
    {

        static inline
        size_t __num_bits_for(const unsigned MSB) throw()
        {
            if(0!=(MSB&0x80)) return 8;
            if(0!=(MSB&0x40)) return 7;
            if(0!=(MSB&0x20)) return 6;
            if(0!=(MSB&0x10)) return 5;
            if(0!=(MSB&0x08)) return 4;
            if(0!=(MSB&0x04)) return 3;
            if(0!=(MSB&0x02)) return 2;
            if(0!=(MSB&0x01)) return 1;
            return 0;
        }

        size_t natural::bits() const throw()
        {
            YOCTO_CHECK_MPN(*this);
            if(size>0)
            {
                const size_t   imsb = size-1;
                const unsigned MSB  = byte[imsb]; assert(0!=MSB);
                return (imsb<<3) + __num_bits_for(MSB);
            }
            return 0;
        }

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

        void natural:: put(ios::bitio &Q) const
        {
            if(size>0)
            {
                const size_t imsb = size-1;
                for(size_t i=0;i<imsb;++i)
                {
                    const unsigned B = byte[i];
                    for(size_t j=0;j<8;++j)
                    {
                        Q.push(0 != (B&_bit[j]));
                    }
                }
                const unsigned MSB = byte[imsb];          assert(0!=MSB);
                const size_t   num = __num_bits_for(MSB); assert(num>0);
                for(size_t j=0;j<num;++j)
                {
                    Q.push(0 != (MSB&_bit[j]));
                }
            }
        }

        void natural:: put(ios::bitio &Q, const size_t n) const
        {
            const size_t num_bits = bits();
            size_t obits = n;
            size_t ebits = 0;
            if(n>num_bits)
            {
                obits = num_bits;
                ebits = n-num_bits;
            }
            for(size_t i=0;i<obits;++i)
            {
                Q.push( 0!= (byte[i>>3]&_bit[i&7]) );
            }
            for(size_t i=0;i<ebits;++i)
            {
                Q.push(false);
            }

        }

        natural natural::get(ios::bitio &Q, const size_t n)
        {
            assert(n<=Q.size());
            if(n<=0)
            {
                return natural();
            }
            else
            {
                const size_t nb = YOCTO_ROUND8(n)>>3;
                natural ans(nb,as_capacity);
                for(size_t ibit=0;ibit<n;++ibit)
                {
                    if( Q.pop() )
                    {
                        ans.byte[ibit>>3] |= _bit[ibit&7];
                    }
                }
                ans.update();
                return ans;
            }
        }


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
            natural ans=1;
            while(--nbits>0)
            {
                ans.shl().byte[0] |= static_cast<uint8_t>( manager::random_bit() ? 1 : 0 );
            }
            return ans;
        }
    }
}


