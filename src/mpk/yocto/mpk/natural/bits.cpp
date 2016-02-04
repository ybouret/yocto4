#include "yocto/mpk/natural.hpp"

namespace yocto {

    namespace mpk
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

namespace yocto
{

    namespace mpk
    {

        natural natural:: randomize(size_t num_bits)
        {
            return natural();
        }

    }

}
