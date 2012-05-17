#include "yocto/lexical/unit.hpp"

namespace yocto 
{
    namespace lexical 
    {
        unit:: unit( const string &t,size_t l) throw():
        token(),
        type(t),
        line(l),
        next(0),
        prev(0)
        {
        }
        
        unit:: ~unit() throw() 
        {
            
        }
        
    }
    
}
