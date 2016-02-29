#include "yocto/mpa/word2mpn.hpp"

namespace yocto
{
    
    namespace mpa
    {
        
        natural natural::power(const natural &lhs, const natural &e )
        {
            natural ans = one();
            natural n   = e;
            while(n>0)
            {
                ans *= lhs;
                --n;
            }
            return ans;
        }
        
        natural natural:: power( const natural &lhs, const uint64_t e)
        {
            const word2mpn w(e);
            return power(lhs,w.n);
        }
    }

}

