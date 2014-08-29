#include "yocto/lang/pattern.hpp"

namespace yocto
{
    namespace lang
    {
        pattern:: ~pattern() throw()
        {
            
        }
        
        pattern:: pattern( const uint32_t user_type ) throw() :
        token(),
        next(0),
        prev(0),
        type(user_type)
        {
        }
        
        void pattern:: reset() throw() { clear(); }
        
    }
}