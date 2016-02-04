#include "yocto/mpk/natural.hpp"
#include "yocto/mpk/word2mpn.hpp"
#include "yocto/code/unroll.hpp"

namespace yocto
{
    namespace mpk
    {

        bool natural::are_equal(const natural &lhs, const natural &rhs) throw()
        {
            const size_t n = lhs.size;
            if(rhs.size==n)
            {
                const uint8_t *l = lhs.byte;
                const uint8_t *r = rhs.byte;
                YOCTO_LOOP(n,if( *(l++) != *(r++) ) return false);
                return true;
            }
            else
            {
                return false;
            }
        }

        YOCTO_MPN_IMPL2_NOTHROW(bool,operator==,natural::are_equal)

        bool natural::are_different(const natural &lhs, const natural &rhs) throw()
        {
            const size_t n = lhs.size;
            if(rhs.size==n)
            {
                const uint8_t *l = lhs.byte;
                const uint8_t *r = rhs.byte;
                YOCTO_LOOP(n,if( *(l++) != *(r++) ) return true);
                return false;
            }
            else
            {
                return true;
            }
        }

        YOCTO_MPN_IMPL2_NOTHROW(bool,operator!=,natural::are_different)


        int natural:: compare(const natural &lhs, const natural &rhs) throw()
        {
            YOCTO_CHECK_MPN(lhs);
            YOCTO_CHECK_MPN(rhs);
            const size_t nl = lhs.size;
            const size_t nr = rhs.size;
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
                    // same size
                    const uint8_t *l = lhs.byte+nl;
                    const uint8_t *r = rhs.byte+nr;
                    YOCTO_LOOP(nl,
                               const int L = *(--l);
                               const int R = *(--r);
                               if(L<R) { return -1; } else { if(R<L) { return 1; } }
                               );
                    return 0;
                }
            }
        }


        int natural::compare(const natural &lhs, const word_t   rhs) throw()
        {
            const word2mpn RHS(rhs);
            return compare(lhs,RHS.n);
        }

        int natural::compare(const word_t   lhs, const natural &rhs) throw()
        {
            const word2mpn LHS(lhs);
            return compare(LHS.n,rhs);
        }
        

    }

}

