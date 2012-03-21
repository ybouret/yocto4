#include "yocto/utest/run.hpp"
#include "yocto/rx/lexer.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/rx/source.hpp"

using namespace yocto;


namespace 
{
    
    class MyLexer : public regex::lexer
    {
    public:
        typedef regex::lexical::action callback;
        
        MyLexer() : 
        regex::lexer("main"),
        iline(1)
        {
            regex::sub_lexer &lex = main();
            const callback   __show( this, & MyLexer::show );
            const callback   __endl( this, & MyLexer::endl );
            const callback   __drop( this, & MyLexer::discard);
            lex.make( "[:digit:]+",  __show );
            lex.make( "[:digit:]+f", __show );
            lex.make( "[:endl:]",    __endl );
            lex.make( ".",           __drop );
            
        }
        
        
        
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
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyLexer);
    };
    
}

YOCTO_UNIT_TEST_IMPL(lex)
{
    
    MyLexer Lex;
   
    
    ios::icstream fp( ios::cstdin );
    regex::source src( fp );
    while( Lex.main().process(src) )
    {
    }
    
}
YOCTO_UNIT_TEST_DONE()
