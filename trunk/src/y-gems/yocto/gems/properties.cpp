#include "yocto/gems/properties.hpp"

namespace yocto
{
    namespace gems
    {
        
        
        properties:: ~properties() throw() 
        {
        }
     
        properties:: properties( word_t t, const string &n ) :
        type(t),
        name(n)
        {
        }
        
        
        const word_t & properties:: key()    const throw() { return type; }
        const string & properties:: subkey() const throw() { return name; }
        
    }
    
}
