#include "yocto/lang/lexer.hpp"

namespace yocto
{
    namespace lang
    {
    
        namespace lexical
        {
            
            plugin:: ~plugin() throw()
            {
            }
            
            plugin:: plugin( const string &id, lexer &parent ) :
            scanner(id, parent.line, parent.dict )
            {
                link_to(parent);
                
            }
            
        }
        
        
    }
}