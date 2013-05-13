#include "yocto/lingua/pattern.hpp"

namespace yocto
{
    
    namespace lingua
    {
        
        pattern:: ~pattern() throw() {}
        
        
        pattern:: pattern() throw()  :
        prev(0),
        next(0)
        {}
        
        void pattern:: reset() throw()
        {
            clear();
        }
        
        
    }
    
}
