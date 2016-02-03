#include "yocto/mpk/natural.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/unroll.hpp"

namespace yocto
{
    namespace mpk
    {

        natural natural:: add(const _natural &lhs, const _natural &rhs)
        {
            size_t         na = lhs.size;
            size_t         nb = rhs.size;
            const uint8_t *a  = lhs.byte;
            const uint8_t *b  = rhs.byte;
            
            // a is the shortest
            if(na>nb)
            {
                cswap(na,nb);
                cswap(a,b);
            }
            
            // prepare answer
            const size_t ns    = nb+1;
            natural      ans(ns,as_capacity);
            uint8_t     *s     = ans.byte;
            
            // loop with carray
            unsigned     carry = 0;
            
            // common part
            YOCTO_LOOP(na,
                       carry += unsigned(*(a++)) + unsigned( *(b++) );
                       *(s++) = uint8_t(carry);
                       carry >>= 8;
                       );
            
            // propagate carry
            YOCTO_LOOP(nb-na,
                       carry += unsigned( *(b++) );
                       *(s++) = uint8_t(carry);
                       carry >>= 8;
                       );
            
            *s = uint8_t(carry);
            ans.size = ns;
            ans.update();
            return ans;
        }
    }

}
