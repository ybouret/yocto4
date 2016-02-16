#include "yocto/mpl/natural.hpp"
#include "yocto/code/unroll.hpp"

namespace yocto
{
    namespace mpl
    {

        int natural::compare(const void  *lhs,
                             const size_t nl,
                             const void  *rhs,
                             const size_t nr ) throw()
        {
            if(nl<nr)
            {
                return -1;
            }
            else
            {
                if(nr<nl)
                {
                    return 1;
                }
                else
                {
                    const uint8_t *l = (const uint8_t *)lhs;
                    const uint8_t *r = (const uint8_t *)rhs;
                    YOCTO_LOOP(nl,
                               const unsigned L = *(l++);
                               const unsigned R = *(r++);
                               if(L<R)
                               {
                                   return -1;
                               }
                               else
                               {
                                   if(R<L)
                                   {
                                       return 1;
                                   }
                                   // else equality=>continue;
                               }
                               );
                    return 0;
                }
            }
        }
    }

}
