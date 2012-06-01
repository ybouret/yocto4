#include "yocto/lang/grammar.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        grammar:: ~grammar() throw()
        {
        }
        
        grammar:: grammar( const string &id ) :
        name(id)
        {
        }
        
    }
    
}