#include "yocto/lang/syntax/aggregate.hpp"

namespace yocto 
{
    namespace lang
    {
        namespace syntax 
        {
            
            aggregate:: ~aggregate() throw() {}
            
            aggregate:: aggregate( const string &id ) : compound(id) 
            {
            }
            
                     
            bool  aggregate:: match( Y_SYNTAX_MATCH_ARGS )
            {
                std::cerr << "?AGG <" << label << ">=[ ";
                for( size_t i=1; i <= operands.size(); ++i )
                    std::cerr << operands[i]->label << " ";
                std::cerr << "]" << std::endl;
                
                //--------------------------------------------------------------
                // make a sub-tree
                //--------------------------------------------------------------
                parse_node *sub_tree =  new parse_node(this->label,NULL);
                
                try 
                {
                    //----------------------------------------------------------
                    // try to fill it with the operands
                    //----------------------------------------------------------
                    const size_t nr = operands.size();
                    for( size_t i=1; i <= nr; ++i )
                    {
                        rule *curr = operands[i];
                        if( !curr->match(Lexer, Source, sub_tree) )
                        {
                            parse_node::restore(Lexer,sub_tree);
                            return false;
                        }
                    }
                    
                    //----------------------------------------------------------
                    // OK
                    //----------------------------------------------------------
                    grow(Tree,sub_tree);
                    return true;
                }
                catch(...)
                {
                    if( sub_tree ) delete sub_tree;
                    throw;
                }
            }
            
            
        }
    }
    
}
