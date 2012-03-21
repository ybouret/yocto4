#include "yocto/utest/run.hpp"
#include "yocto/rx/lexical/sub-lexer.hpp"


using namespace yocto;


namespace 
{
    
    struct dummy
    {
   
        void show( const regex::token &p ) 
        {
            std::cerr << "<" << p << ">" << std::endl;
        }
    };
    
}

YOCTO_UNIT_TEST_IMPL(lex)
{
    typedef regex::lexical::action callback;
    dummy            dum;
    regex::sub_lexer lex("toto");
    lex.make( "[:digit:]+", callback( &dum, & dummy::show ) );
}
YOCTO_UNIT_TEST_DONE()
