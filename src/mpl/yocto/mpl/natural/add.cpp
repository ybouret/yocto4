
#include "yocto/mpl/natural.hpp"
#include "yocto/code/unroll.hpp"

namespace yocto
{
    namespace mpl
    {

        natural natural:: add(const void    *lhs, size_t nl,
                              const void    *rhs, size_t nr)
        {
            if(nl>nr)
            {
                cswap(nl, nr );
                cswap(lhs,rhs);
            }
            assert(nl<=nr);

            //__________________________________________________________________
            //
            // nl is the shortest
            //__________________________________________________________________
            const size_t   ns = nr+1;
            natural        ans(ns,as_capacity);
            uint8_t       *s  = ans.byte;
            const uint8_t *l = (const uint8_t *)lhs;
            const uint8_t *r = (const uint8_t *)rhs;
            unsigned carry = 0;

            //__________________________________________________________________
            //
            // common part
            //__________________________________________________________________
            YOCTO_LOOP(nl,
                       carry += unsigned(*(l++)) + unsigned( *(r++) );
                       *(s++) = uint8_t(carry);
                       carry >>= 8);

            //__________________________________________________________________
            //
            // extra part
            //__________________________________________________________________
            YOCTO_LOOP(nr-nl,
                       carry += unsigned( *(r++) );
                       *(s++) = uint8_t(carry);
                       carry >>= 8);

            //__________________________________________________________________
            //
            // report carry
            //__________________________________________________________________
            *s = uint8_t(carry);

            //__________________________________________________________________
            //
            // done
            //__________________________________________________________________
            ans.update();
            return ans;
        }
        
        
    }
    
}
