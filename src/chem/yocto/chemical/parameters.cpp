#include "yocto/chemical/parameters.hpp"

namespace yocto
{
    
    namespace chemical
    {
        parameters:: ~parameters() throw()
        {
            (void)lib.liberate();
        }
        
    }
    
}
