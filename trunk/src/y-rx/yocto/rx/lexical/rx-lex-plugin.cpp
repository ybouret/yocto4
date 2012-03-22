#include "yocto/rx/lexical/plugin.hpp"


namespace yocto 
{
    namespace regex
    {
        
        namespace lexical
        {
            plugin:: ~plugin() throw()
            {
            }
            
            plugin:: plugin( const char *id ) : regex::sublexer(id)
            {
            }
            
        }
    }
}

