#include "yocto/mpk/word2mpn.hpp"
#include "yocto/string.hpp"
#include "yocto/code/bswap.hpp"
#include <iostream>

namespace yocto
{
    namespace mpk
    {

        static inline bool is0(char C) throw()
        {
            return C == '0';
        }

        std::ostream & operator<<( std::ostream &os, const natural &n)
        {
            YOCTO_CHECK_MPN(n);
            std::ios_base::fmtflags flags = os.flags();
            if( flags &  std::ios::hex)
            {
                if(n.size<=0)
                {
                    os << "0x0";
                }
                else
                {
                    string s;
                    for(size_t i=0;i<n.size;++i)
                    {
                        const uint8_t B  = n.byte[i];
                        const uint8_t lo = B      & 0x0f;
                        s.append( hexa_char[lo]);
                        const uint8_t hi = (B>>4) & 0x0f;
                        s.append(hexa_char[hi]);
                    }
                    s.trim(is0);
                    mreverse((char*)(s.rw()),s.length());
                    os << s;
                }
            }
            else
            {
                //assume decimal
                if(n.size<=0)
                {
                    os << "0";
                }
                else
                {
                    const natural ten(10);
                    natural       q;
                    natural       r;
                    natural       x = n;
                    while(x.size>0)
                    {
                        natural::split(q,r,x,ten);
                    }
                }
            }
            return os;
        }
        
    }
    
}
