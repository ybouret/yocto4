#include "yocto/gfx/types.hpp"

namespace yocto
{
    namespace gfx
    {
        string __conv::b2s(uint8_t b)
        {
            string ans;
            for(int i=7;i>=0;--i)
            {
                if( 0 != (b& (1<<i)))
                    ans += '1';
                else
                    ans += '0';
            }
            return ans;
        }
    }
}
