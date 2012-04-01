#include "yocto/rx/lexical/mod-cstring.hpp"

#include "yocto/rx/pattern/logic.hpp"
#include "yocto/rx/pattern/basic.hpp"

#include "yocto/auto-ptr.hpp"

namespace yocto 
{
    namespace regex
    {
        
        namespace lexical
        {
            
            mod_cstring:: ~mod_cstring() throw()
            {
            }
            
            mod_cstring:: mod_cstring( const callback &cb ) :
            plugin( "C String", "\"", "\"", cb )
            {
                //--------------------------------------------------------------
                //-- single, non escaped chars
                //--------------------------------------------------------------
                {
                    auto_ptr<logical::Operator> q( logical:: OR::create() );
                    
                    *q << basic::range::create(32,33);
                    *q << basic::range::create(35,38);
                    *q << basic::range::create(40,91);
                    *q << basic::range::create(93,126);
                    const action __add( this, &mod_cstring::add );
                    join( q.yield(), __add);
                }
            }
            
            void mod_cstring:: enter()
            {
                content.clear();
            }
            
            void mod_cstring:: leave()
            {
                /* do nothing */
            }
            
            void * mod_cstring::data() throw()
            {
                return &content;
            }
            
            void mod_cstring:: add( const token &tkn )
            {
                for( const t_char *t = tkn.head; t; t=t->next )
                {
                    content.append( t->data );
                }
            }
            
        }
    }
}
