#include "yocto/fovea/shape.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace fovea
    {
        ShapeBase:: ~ShapeBase() throw()
        {
            
        }
        
        ShapeBase:: ShapeBase( size_t nv) :
        vertices(nv)
        {
            if(nv<MIN_VERTICES||nv>MAX_VERTICES)
                throw exception("Invalid Shape #VERTEX=%u", unsigned(vertices) );
                
        }

    }
    
    
}
