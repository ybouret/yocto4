
#include "yocto/lang/grammar.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    namespace lang 
    {
        
        
        ////////////////////////////////////////////////////////////////////////
        //
        // grammar accept
        //
        ////////////////////////////////////////////////////////////////////////
        syntax::parse_node *grammar:: accept( lexer &Lexer, regex::source &Source )
        {
            if( rules.size <= 0 )
                throw exception("empty grammar '%s'", name.c_str() );
            
            syntax::parse_node *Tree = NULL;
            if( rules.head->match(Lexer,Source,Tree) ) 
            {
                std::cerr << "[[ SUCCESS ]]" << std::endl;
                return Tree; // may be NULL
            }
            
            std::cerr << "[[ FAILURE ]]" << std::endl;
            return NULL;
        }
        
    }
    
}
