#include "yocto/lang/pattern.hpp"

namespace yocto
{
    namespace lang
    {
        pattern:: pattern(uint32_t ID) throw() : uuid(ID)
        {
        }


        pattern:: ~pattern() throw()
        {

        }

        pattern:: pattern(const pattern &other) throw() :
        uuid(other.uuid)
        {}

        
        
    }
}
