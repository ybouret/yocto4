#include "yocto/mpk/word2mpn.hpp"
#include "yocto/code/unroll.hpp"
#include "yocto/exceptions.hpp"
#include <cerrno>

#include <iostream>

namespace yocto
{
    namespace mpk
    {
        natural natural:: sub( const natural &lhs, const natural &rhs )
        {
            const size_t nl = lhs.size;
            const size_t nr = rhs.size;
            if(nl<nr)
            {
                throw libc::exception(EDOM,"mpk.sub(lhs>rhs) [level-1]");
            }

            // nl>=nr
            natural        ans(nl,as_capacity);
            const uint8_t *l = lhs.byte;
            const uint8_t *r = rhs.byte;
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
                throw libc::exception(EDOM,"mpk.sub(lhs>rhs) [level-2]");
            }

            ans.size = nl;
            ans.update();
            return ans;
        }


        YOCTO_MPN_IMPL2(natural,operator-,natural::sub)

        natural & natural:: operator-=(const word_t rhs)
        {
            const word2mpn RHS(rhs);
            natural        ans = sub(*this,RHS.n);
            xch(ans);
            return *this;
        }

        natural &  natural:: operator--()
        {
            const word2mpn __one(1);
            natural ans = sub(*this,__one.n);
            xch(ans);
            return *this;
        }

        natural   natural:: operator-- (int)
        {
            const word2mpn __one(1);
            const natural ans = sub(*this,__one.n);
            *this = ans;
            return ans;
        }


    }

}
