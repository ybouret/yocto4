#include "yocto/mpa/natural.hpp"
#include <iostream>
#include "yocto/code/utils.hpp"

namespace yocto
{
    namespace mpa
    {
        
        std::ostream & operator<<( std::ostream &os, const natural &n )
        {
            size_t i=n.size;
            if(i)
            {
                const uint8_t msb = n.byte[--i];
                
                if( msb & 0xf0 )
                    os << hexa_text[msb];
                else
                    os << hexa_char[msb&0xf];
                
                while(i-- > 0)
                {
                    const uint8_t b = n.byte[i];
                    os << hexa_text[b];
                }
            }
            else
                os << "0";
            return os;
        }
    }
    
}

