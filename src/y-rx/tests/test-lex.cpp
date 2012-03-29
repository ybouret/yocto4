#include "yocto/utest/run.hpp"
#include "yocto/rx/lexer.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/rx/source.hpp"
#include "yocto/rx/lexical/mod-ccomment.hpp"

using namespace yocto;


namespace 
{
    
    class MyLexer : public regex::lexer
    {
    public:
        typedef regex::lexical::action callback;
        
        MyLexer() : 
        regex::lexer("main"),
        iline(1),
        plugCom( NULL )
        {
            regex::sublexer &lex = main();
            
            load( plugCom = new regex::lexical::mod_ccomment() );
            
            const callback   __show( this, & MyLexer::show );
            const callback   __endl( this, & MyLexer::endl );
            const callback   __drop( this, & MyLexer::discard);
            const callback   __comment( this, & MyLexer::ini_comment );
            
            lex.make( "[:digit:]+",  __show );
            lex.make( "[:digit:]+f", __show );
            lex.make( "[:endl:]",    __endl );
            lex.call( "comment", "//", __comment );
            lex.plug( plugCom->name );
            lex.make( ".", this, & MyLexer::discard);
            
            regex::sublexer &com = declare( "comment" );
            
            com.back( "[:endl:]", __endl );
            com.make( ".",        __drop );
                        
        }
        
        
        
        size_t                        iline;
        regex::lexical::mod_ccomment *plugCom;
        void show( const regex::token &p ) 
        {
            std::cerr << "<" << p << ">" << std::endl;
        }
        
        void discard(  const regex::token &p ) throw(){}
        
        void endl( const regex::token &p )
        {
            std::cerr << "[#" << iline++ << "]" << std::endl;
        }
        
        void ini_comment( const regex::token & )
        {
            std::cerr << "[COMMENT]" << std::endl;
        }
        
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyLexer);
    };
    
}

YOCTO_UNIT_TEST_IMPL(lex)
{
    
    MyLexer Lex;
    Lex.reset();
    
        
    ios::icstream fp( ios::cstdin );
    regex::source src( fp );
    Lex.run(src);
   
    
}
YOCTO_UNIT_TEST_DONE()
