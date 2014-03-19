#include "yocto/mpa/natural.hpp"

namespace yocto
{
    namespace mpa
    {
        
        natural natural:: rand(size_t nbits)
        {
            if(nbits)
            {
                natural ans;
                for(size_t i=nbits;i>0;--i)
                {
                    ans <<= 1;
                    if( memIO::random_bit() )
                    {
                        ans.byte[0] |= 1;
                        ans.rescan();
                    }
                }
                return ans;
            }
            else
                return natural();
        }
        
    }
    
}
