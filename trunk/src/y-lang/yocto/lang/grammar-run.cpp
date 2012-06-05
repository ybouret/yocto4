
#include "yocto/lang/grammar.hpp"
#include "yocto/exception.hpp"
#include "yocto/ios/ocstream.hpp"

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
                if( Tree )
                {
                    ios::ocstream fp( "g.dot", false );
                    Tree->graphviz("G",fp);
                }
                return Tree; // may be NULL
            }
            
            return NULL;
        }
        
    }
    
}
