#include "yocto/fovea/cell.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace fovea
    {
        Shape:: ~Shape() throw()
        {
            
        }
        
        Shape:: Shape( size_t nv) :
        vertices(nv)
        {
            if(nv<MIN_VERTICES||nv>MAX_VERTICES)
                throw exception("Invalid Shape #VERTEX=%u", unsigned(vertices) );
                
        }

    }
    
    
}
