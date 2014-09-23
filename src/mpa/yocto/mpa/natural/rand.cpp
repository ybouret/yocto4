#include "yocto/mpa/natural.hpp"

namespace yocto
{
    namespace mpa
    {
        
        natural natural:: rand(size_t nbits)
        {
            if(0!=nbits)
            {
                size_t nbytes = nbits/8;
                if(8*nbytes<nbits) ++nbytes;
                
                natural ans(nbytes,as_capacity);
                for(size_t i=0;i<nbits;++i)
                {
                    if( memIO::random_bit())
                    {
                        ans.byte[i>>3] |= (1 << (i&7) );
                    }
                }
                
#if 0
                for(size_t i=nbits;i>0;--i)
                {
                    ans <<= 1;
                    if( memIO::random_bit() )
                    {
                        ans.byte[0] |= 1;
                        ans.rescan();
                    }
                }
#endif
                
                ans.rescan();
                return ans;
            }
            else
            {
                return natural();
            }
        }
        
    }
    
}
