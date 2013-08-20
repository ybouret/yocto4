#include "yocto/gfx/types.hpp"

namespace yocto
{
    namespace gfx
    {
        string conv::to_binary(const uint8_t &b)
        {
            string ans(8,as_capacity);
            for(int shift=7;shift>=0;--shift)
            {
                if( b & (1<<shift) )
                    ans += '1';
                else
                    ans += '0';
            }
            return ans;
        }
        
        string conv::to_binary(const uint32_t &b)
        {
            string ans(32,as_capacity);
            for(int shift=31;shift>=0;--shift)
            {
                if( b & (1<<shift) )
                    ans += '1';
                else
                    ans += '0';
            }
            return ans;
        }

        
    }
}
