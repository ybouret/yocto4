#include "yocto/lang/pattern.hpp"

namespace yocto
{
    namespace lang
    {
        pattern:: pattern(uint32_t ID) throw() : next(0), prev(0), uuid(ID)
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
