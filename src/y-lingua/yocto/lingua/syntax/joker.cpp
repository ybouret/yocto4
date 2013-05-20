#include "yocto/lingua/syntax/joker.hpp"

namespace yocto
{
    namespace lingua
    {
        namespace syntax
        {
            
            ////////////////////////////////////////////////////////////////////
            //
            //
            //
            ////////////////////////////////////////////////////////////////////
            joker:: ~joker() throw() {}
            
            joker:: joker( const string &id, rule &r) :
            rule(id),
            handle( &r )
            {
            }
            
            
            ////////////////////////////////////////////////////////////////////
            //
            //
            //
            ////////////////////////////////////////////////////////////////////
            optional:: optional( const string &id, rule &r) : joker(id,r)
            {
            }
            
            optional:: ~optional() throw()
            {
            }
            
            Y_SYNTAX_MATCH_TYPE optional:: match( Y_SYNTAX_MATCH_ARGS)
            {
                check(Tree);
                assert(handle);
                std::cerr << "?OPT " << label << ": " << handle->label << std::endl;
                xnode *subTree = 0;
                if( handle->match(Lexer, Source, subTree) && subTree != 0)
                {
                    grow(Tree, subTree);
                }
                return true;
            }
            
        }
    }
}

