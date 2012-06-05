#include "yocto/lang/syntax/joker.hpp"

namespace yocto 
{
    namespace lang 
    {
        namespace syntax
        {

            joker:: joker( const string &id, rule &r ) :
            rule(id),
            ref(r)
            {
            }
            
            joker:: ~joker() throw()
            {
            }
            
            ////////////////////////////////////////////////////////////////////
            //
            // optional (?)
            //
            ////////////////////////////////////////////////////////////////////
            optional:: optional( const string &id, rule &r ) :
            joker( id, r )
            {
            }
            
            optional:: ~optional() throw()
            {
            }
            
            bool optional:: match( Y_SYNTAX_MATCH_ARGS )
            {
                std::cerr << "?OPT <" << ref.label << "> (" << label << ")" << std::endl;
                parse_node *Node = NULL;
                
                if( ref.match(Lexer, Source, Node) )
                {
                    grow(Tree,Node);
                }
                
                return true;
            }
            
        }
    }

}
