#include "yocto/lang/ast.hpp"
#include <cstring>

namespace yocto 
{
    namespace lang 
    {
        
        AST:: ~AST() throw()
        {
        }
        
        AST:: AST( const syntax::parse_node &node ) :
        label( node.label ),
        next(0),
        prev(0),
        terminal( node.terminal != 0 ),
        wksp()
        {
            memset( wksp, 0, sizeof(wksp));
            if( terminal )
            {
                const lexeme *lx = node->lex();
            }
           
        }
        
        
        
    }

}
