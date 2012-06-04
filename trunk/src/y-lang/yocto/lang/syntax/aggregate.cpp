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
            
            aggregate:: aggregate( const aggregate &other ) : compound( other )
            {
            }
            
            
            rule * aggregate:: clone() const
            {
                return new aggregate( *this );
            }
            
            bool  aggregate:: match( Y_SYNTAX_MATCH_ARGS )
            {
                std::cerr << "?AGG <" << label << ">" << std::endl;
                //--------------------------------------------------------------
                // make a sub-tree
                //--------------------------------------------------------------
                parse_node *sub_tree =  new parse_node(this->label,NULL);
                
                try 
                {
                    //----------------------------------------------------------
                    // try to fill it with the operands
                    //----------------------------------------------------------
                    for( rule *curr = operands.head; curr; curr = curr->next )
                    {
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
