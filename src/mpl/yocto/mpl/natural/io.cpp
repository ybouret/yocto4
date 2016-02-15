#include "yocto/mpl/natural.hpp"
#include "yocto/string.hpp"
#include "yocto/code/utils.hpp"
#include "yocto/code/bswap.hpp"
#include <iostream>

namespace yocto
{
    namespace mpl
    {

        static inline bool is0(char C) throw() { return '0' == C; }

        void natural:: display( std::ostream &os ) const
        {
            if(size<=0)
            {
                os << '0';
            }
            else
            {
                string s;
                for(size_t i=0;i<size;++i)
                {
                    const unsigned B = byte[i];
                    s.append( hexa_char[B&0xf] );
                    s.append( hexa_char[(B>>4)&0xf]);
                }
                s.trim(is0);
                mreverse((char*)(s.rw()),s.length());
                os << s;
            }
        }
        
    }
    
}

