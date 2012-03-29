#include "yocto/rx/lexical/mod-ccomment.hpp"
#include "yocto/rx/compiler.hpp"
#include "yocto/rx/pattern/basic.hpp"

#include <iostream>

namespace yocto 
{
    namespace regex
    {
        
        namespace lexical
        {
            
            mod_ccomment:: ~mod_ccomment() throw()
            {
            }
            
            mod_ccomment:: mod_ccomment() :
            plugin( "C Comment", "/\\*", "\\*/" )
            {
                const action __discard( this, & sublexer::discard );
                make( basic::any1::create(), __discard );
            }
            
            void mod_ccomment:: enter() 
            {
                /* do nothing */
                std::cerr << "Start C Comment" << std::endl;
            }
            
            void mod_ccomment:: leave() 
            {
                /* do nothing */
                std::cerr << "End C Comment" << std::endl;
            }
            
        }
    }
}
