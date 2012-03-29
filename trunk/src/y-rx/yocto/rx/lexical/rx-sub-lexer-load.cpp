#include "yocto/rx/lexer.hpp"

namespace yocto
{
    namespace regex
    {

        
        void sublexer:: plug( const string &name )
        {
            assert(parent!=NULL);
            const lexical::plugin &plg = parent->get_plugin(name);
            
            call( plg.name, plg.trigger(), plg.on_call());
            
            
        }

    }

}
