#include "yocto/mpa/natural.hpp"
#include "yocto/code/bswap.hpp"
#include "yocto/code/round.hpp"
#include "yocto/math/types.hpp"

namespace yocto
{
    namespace mpa
    {
        
        int natural:: compare( const natural &lhs, const natural &rhs ) throw()
        {
            YOCTO_CHECK_MPN(&lhs);
            YOCTO_CHECK_MPN(&rhs);
            const size_t L = lhs.size;
            const size_t R = rhs.size;
            if(L<R)
            {
                return -1;
            }
            else
            {
                if(R<L)
                    return 1;
                else
                {
                    const uint8_t *l = lhs.byte;
                    const uint8_t *r = rhs.byte;
                    size_t i = L;
                    while(i-->0)
                    {
                        const int ll = l[i];
                        const int rr = r[i];
                        if(ll!=rr) return ll-rr;
                    }
                    return 0; // all bytes equal
                }
            }
        }
    }

}
