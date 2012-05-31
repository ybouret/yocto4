#include "yocto/lang/syntax/parse-node.hpp"
#include <cstring>

namespace yocto 
{
    namespace lang 
    {
        
        namespace syntax
        {
            parse_node:: ~parse_node() throw()
            {
                if( terminal )
                {
                    lexeme *lx = lex();
                    if( lx ) delete lx;
                }
                else
                {
                    child_list &ch = children();
                    while( ch.size ) delete ch.pop_back();
                }
                memset(wksp,0,sizeof(wksp));
            }
            
            
            parse_node:: parse_node( const string &label_ref, lexeme *lx ) throw() :
            label( label_ref ), 
            terminal( lx != 0 ),
            prev(0),
            next(0), 
            parent(0),
            wksp()
            {
                memset(wksp,0,sizeof(wksp));
                if( lx )
                {
                    lexeme **ppLx = (lexeme **) &wksp[0];
                    *ppLx = lx;
                }
            }
            
            lexeme * parse_node:: lex() throw()
            {
                assert(terminal);
                lexeme **ppLx = (lexeme **) &wksp[0];
                return *ppLx;
            }
            
            parse_node::child_list & parse_node::children() throw()
            {
                assert(!terminal);
                return *(child_list *) &wksp[0];
            }
            
        }
        
    }
}