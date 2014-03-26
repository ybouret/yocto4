#include "yocto/lang/syntax/parse-node.hpp"
#include "yocto/code/cast.hpp"
#include <cstring>

namespace yocto 
{
    namespace lang 
    {
        
        namespace syntax
        {
            
            lexeme * & parse_node:: __lex() throw()
            {
                return *_cast::from<lexeme *>(&wksp[0]);
            }
            
            
            parse_node:: ~parse_node() throw()
            {
                if( terminal!=0 )
                {
                    lexeme * lx = __lex();
                    if( lx ) 
                        delete lx;
                }
                else
                {
                    child_list &ch = children();
                    while( ch.size ) delete ch.pop_back();
                }
                memset(wksp,0,sizeof(wksp));
            }
            
            
#if defined(_MSC_VER)
#pragma warning ( disable : 4351 )
#endif
            parse_node:: parse_node( const string &label_ref, lexeme *lx, node_property ppty ) throw() :
            label( label_ref ), 
            prev(0),
            next(0), 
            parent(0),
            wksp(),
            terminal( uint16_t(lx != 0 ? 1 : 0) ),
            flags(   uint16_t(ppty) )
            {
                memset(wksp,0,sizeof(wksp));
                if( lx )
                {
                    __lex() = lx;
                }
            }
            
            lexeme * parse_node:: lex() throw()
            {
                assert(terminal);
                return __lex();
            }
            
            parse_node::child_list & parse_node::children() throw()
            {
                assert(0==terminal);
                return * _cast::from<child_list>( &wksp[0] );
            }
            
            const lexeme * parse_node:: lex() const throw()
            {
                assert(terminal!=0);
                return *_cast::from<lexeme *>((void*)&wksp[0]);
            }
            
            const parse_node::child_list & parse_node::children() const throw()
            {
                assert(0==terminal);
                return * _cast::from<child_list>( (void*)&wksp[0] );
            }
            
            
            void parse_node:: restore( lexer &Lxr, parse_node *node ) throw()
            {
                assert(node!=NULL);
                if( node->terminal != 0 )
                {
                    lexeme * &lx = node->__lex();
                    assert( lx != NULL );
                    Lxr.unget(lx);
                    lx = 0;
                }
                else 
                {
                    child_list &ch = node->children();
                    while( ch.size ) 
                        restore(Lxr,ch.pop_back());
                }
                delete node;
            }
            
			parse_node *parse_node:: create( const string &label_ref, lexeme *lx, node_property ppty )
			{
				assert(0!=lx);
				try 
				{
					return new parse_node(label_ref,lx,ppty);
				}
				catch(...)
				{
                    delete lx;
					throw;
				}
			}
            
            parse_node *parse_node:: unlink() throw()
            {
                if( parent )
                {
                    assert( !parent->terminal );
                    return parent->children().unlink(this);
                }
                else 
                {
                    return this;
                }
            }
            
            
        }
        
    }
}

