#include "yocto/rx/lexical/mod-cstring.hpp"

#include "yocto/rx/pattern/logic.hpp"
#include "yocto/rx/pattern/basic.hpp"

#include "yocto/auto-ptr.hpp"
#include "yocto/exception.hpp"

#include "yocto/code/utils.hpp"

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
            plugin( "C String", "\"", "\"", cb ),
            content()
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
                
                //--------------------------------------------------------------
                //-- directly escaped chars
                //--------------------------------------------------------------
                {
                    auto_ptr<logical::Operator> q( logical:: AND::create() );
                    *q << basic::single::create( 92 ); // backslash
                    
                    {
                        auto_ptr<logical::Operator> l( logical:: OR::create() );
                        *l << basic::single::create( '\\' );
                        *l << basic::single::create( '"' );
                        *l << basic::single::create( '\'' );
                        *q << l.yield();
                    }
                    const action __add( this, &mod_cstring::esc_add );
                    join( q.yield(), __add );
                }
                
                
                //--------------------------------------------------------------
                //-- indirectly escaped chars
                //--------------------------------------------------------------
                {
                    const action __add( this, &mod_cstring::esc_bin );
                    join( "\\\\[ntrfvba]", __add );
                }
                
                
                
                //--------------------------------------------------------------
                //-- hex escaped sequence
                //--------------------------------------------------------------
                {
                    const action __add( this, &mod_cstring::esc_hex );
                    join( "\\\\x[:xdigit:][:xdigit:]", __add, NULL );
                }
                
                //--------------------------------------------------------------
                //-- invalid hex sequence
                //--------------------------------------------------------------
                {
                    auto_ptr<logical::Operator> q( logical:: AND::create() );
                    *q << basic::single::create( 92 ); // backslash
                    *q << basic::any1::create();       // and another one not met
                    
                    const action __bad( this, &mod_cstring::esc_bad );
                    join( q.yield(), __bad );
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
            
            void mod_cstring:: esc_add( const token &tkn )
            {
                assert( tkn.size == 2 );
                content.append( tkn.tail->data );
            } 
            
            void mod_cstring:: esc_hex( const token &tkn )
            {
                assert( tkn.size == 4 );
                const t_char *t = tkn.tail;
                const int     C = hex2dec( t->data ) + 16 * hex2dec( t->prev->data );
                content.append( char(C) );
            }
            
            void mod_cstring:: esc_bad( const token &tkn )
            {
                assert( tkn.size == 2);
                throw exception("invalid escape sequence '\\%c'", tkn.tail->data);
            }
            
            void mod_cstring:: esc_bin( const token &tkn )
            {
                assert( tkn.size == 2 );
                switch( tkn.tail->data )
                {
                    case 'n': content.append( '\n' ); break;
                    case 'r': content.append( '\r' ); break;
                    case 't': content.append( '\t' ); break;
                    case 'f': content.append( '\f' ); break;
                    case 'v': content.append( '\v' ); break;
                    case 'b': content.append( '\b' ); break;
                    case 'a': content.append( '\a' ); break;
                }
                
            }
            
        }
    }
}
