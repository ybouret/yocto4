#include "yocto/rx/lexical/mod-ccomment.hpp"
#include "yocto/rx/compiler.hpp"

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
            plugin( "C Comment", "/*", "*/" )
            {
            }
            
            void mod_ccomment:: enter() 
            {
                /* do nothing */
            }
            
            void mod_ccomment:: leave() 
            {
                /* do nothing */
            }
                       
        }
    }
}
