#include "yocto/gems/properties.hpp"

namespace yocto
{
    namespace gems
    {
        
        const word_t & properties:: key() const throw() { return type; }
        
        properties:: properties( word_t t, const string &n ) :
        type(t), name(n)
        {
        }
        
        properties:: ~properties() throw() 
        {
        }
        
    }
    
}
