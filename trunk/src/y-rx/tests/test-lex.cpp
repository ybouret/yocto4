#include "yocto/utest/run.hpp"
#include "yocto/rx/lexical/sub-lexer.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/rx/source.hpp"

using namespace yocto;


namespace 
{
    
    struct dummy
    {
   
        size_t iline;
        
        void show( const regex::token &p ) 
        {
            std::cerr << "<" << p << ">" << std::endl;
        }
        
        void discard(  const regex::token &p ) throw(){}
        
        void endl( const regex::token &p )
        {
            std::cerr << "[#" << iline++ << "]" << std::endl;
        }
    };
    
}

YOCTO_UNIT_TEST_IMPL(lex)
{
    typedef regex::lexical::action callback;
    dummy            dum = { 1 };
    regex::sub_lexer lex("toto");
    lex.make( "[:digit:]+",  callback(  &dum, & dummy::show ) );
    lex.make( "[:digit:]+f", callback(  &dum, & dummy::show ) );
    lex.make( "[:endl:]",    callback(  &dum, & dummy::endl ) );
    lex.make( ".",           callback(  &dum, & dummy::discard ) );
    
    ios::icstream fp( ios::cstdin );
    regex::source src( fp );
    while( lex.process(src) )
    {
    }
    
}
YOCTO_UNIT_TEST_DONE()
