#include "yocto/gems/identifier.hpp"

namespace yocto
{
    namespace gems
    {
     
        identifier :: ~identifier() throw()
        {
        }
        
        
        identifier:: identifier( word_t u, word_t t) throw() :
        uuid(u),
        type(t)
        {
        }
        
        
        const word_t & identifier:: key() const throw()
        {
            return uuid;
        }
        
    }
}

