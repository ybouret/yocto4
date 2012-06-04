#include "yocto/lang/syntax/alternate.hpp"

namespace yocto 
{
    namespace lang
    {
        namespace syntax 
        {
            
            alternate:: ~alternate() throw() {}
            
            alternate:: alternate( const string &id ) : compound(id) 
            {
            }
            
            alternate:: alternate( const alternate &other ) :
            compound( other )
            {
            }
            
            
            rule * alternate:: clone() const { return new alternate( *this ); }
            
            bool  alternate:: match( Y_SYNTAX_MATCH_ARGS )
            {
                std::cerr << "?ALT <" << label << ">#=" << operands.size << std::endl;
                
                parse_node *sub_tree = NULL;
                //----------------------------------------------------------
                // try to fill it with the operands
                //----------------------------------------------------------
                for( rule *curr = operands.head; curr; curr = curr->next )
                {
                    if( curr->match(Lexer,Source,sub_tree) )
                    {
                        assert( sub_tree != NULL);
                        grow(Tree,sub_tree);
                        return true;
                    }
                }
                
                //----------------------------------------------------------
                // not match
                //----------------------------------------------------------
                return false;
            }
            
            
        }
    }
    
}
