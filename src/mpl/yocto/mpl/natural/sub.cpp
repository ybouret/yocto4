#include "yocto/mpl/natural.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

namespace yocto
{
    namespace mpl
    {
        natural natural:: sub(const void *lhs, const size_t nl,
                              const void *rhs, const size_t nr)
        {
            if(nl<nr)
            {
                throw libc::exception(EDOM,"mpl.sub(lhs>rhs) [level-1]");
            }

            // nl>=nr
            natural        ans(nl,as_capacity);
            const uint8_t *l = (const uint8_t *)lhs;
            const uint8_t *r = (const uint8_t *)rhs;
            uint8_t       *s = ans.byte;

            //__________________________________________________________________
            //
            // firt loop: common bytes
            //__________________________________________________________________
            ptrdiff_t   carry = 0;
            for( size_t i=nr; i>0; --i )
            {
                const ptrdiff_t a = *(l++); // L[i];
                const ptrdiff_t b = *(r++); // R[i];
                ptrdiff_t       d = (a-b)-carry;
                if( d < 0 )
                {
                    d    += 256;
                    carry = 1;
                }
                else
                {
                    carry = 0;
                }
                assert(d>=0);
                assert(d<256);
                *(s++) = static_cast<uint8_t>(d);
            }


            //__________________________________________________________________
            //
            // second loop: propagate carry
            //__________________________________________________________________
            for( size_t i=nl-nr; i>0; --i )
            {
                const ptrdiff_t a = *(l++);  //L[i];
                ptrdiff_t       d = a-carry;
                if( d < 0 )
                {
                    d    += 256;
                    carry = 1;
                }
                else
                {
                    carry = 0;
                }
                assert(d>=0);
                assert(d<256);
                *(s++) = static_cast<uint8_t>(d);
            }

            if(carry!=0)
            {
                throw libc::exception(EDOM,"mpl.sub(lhs>rhs) [level-2]");
            }

            ans.update();
            return ans;
        }
    }
    
}
