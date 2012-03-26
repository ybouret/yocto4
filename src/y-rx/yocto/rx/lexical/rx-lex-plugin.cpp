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
            
            plugin:: plugin( const char *id ) : 
            regex::sublexer(id),
            __Enter( this, & plugin::on_enter ),
            __Leave( this, & plugin::on_leave )
            {
            }
            
            
            void plugin:: on_enter( const token & )
            {
            }
            
            void plugin:: on_leave( const token & )
            {
            }
            
            const action & plugin:: enter() const throw() { return __Enter; }
            const action & plugin:: leave() const throw() { return __Leave; }
            
            
        }
    }
}

