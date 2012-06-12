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
            
            bool  alternate:: match( Y_SYNTAX_MATCH_ARGS )
            {
#if 0
                std::cerr << "?ALT <" << label << ">=[ ";
                for( size_t i=1; i <= operands.size(); ++i )
                    std::cerr << operands[i]->label << " ";
                std::cerr << "]" << std::endl;
#endif
                
                check(Tree);
                parse_node *sub_tree = NULL;
                //----------------------------------------------------------
                // try to fill it with the operands
                //----------------------------------------------------------
                const size_t nr = operands.size();
                for( size_t i=1; i <= nr; ++i )
                {
                    rule *curr      = operands[i];
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
            
            alternate & alternate:: operator|=( rule &r )
            {
                (*this)(r);
                return *this;
            }
        }
    }
    
}
