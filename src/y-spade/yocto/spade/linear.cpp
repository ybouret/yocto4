#include "yocto/spade/linear.hpp"

namespace yocto
{
    
    namespace spade
    {
        
        linear :: ~linear() throw() {}
        
        linear:: linear( size_t num_bytes ) throw() : bytes(num_bytes)
        {
            assert(bytes>0);
        }
    }
    
}

