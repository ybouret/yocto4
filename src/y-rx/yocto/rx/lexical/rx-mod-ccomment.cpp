#include "yocto/rx/lexical/mod-ccomment.hpp"
#include "yocto/rx/compiler.hpp"
//#include "yocto/rx/pattern/basic.hpp"

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
            
            mod_ccomment:: mod_ccomment( const callback &cb ) :
            plugin( "C Comment", "/\\*", "\\*/", cb),
            content()
            {
                make( "[:endl:]", this, & mod_ccomment::add, NULL );
                make( ".",        this, & mod_ccomment::add, NULL );
            }
            
            void mod_ccomment:: enter() 
            {
                /* do nothing */
                std::cerr << "Start C Comment" << std::endl;
                content.clear();
            }
            
            void mod_ccomment:: leave() 
            {
                /* do nothing */
                std::cerr << "End C Comment" << std::endl;
            }
         
            void *mod_ccomment:: data() throw()
            {
                return &content;
            }

            
            void mod_ccomment:: add( const token &tkn )
            {
                for(const t_char *t = tkn.head; t; t = t->next )
                {
                    content.append( t->data );
                }
            }
            
        }
    }
}
