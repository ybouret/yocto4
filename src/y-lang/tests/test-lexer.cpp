#include "yocto/utest/run.hpp"
#include "yocto/lang/lexer.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;

namespace {
    
    class MyLex : public lang::lexer
    {
    public:
        explicit MyLex() : lang::lexer( "MyLex" )
        {
            lang::lexical::scanner &scan = first();
            
            scan.make( "INT",    "[:digit:]+" );
            scan.make( "BLANKS", "[ \t]",    & scan.discard );
            scan.make( "ENDL",   "[:endl:]", & scan.newline );
            
            {
                const lang::lexical::callback __CommentC( this, & MyLex::OnCommentC);
                scan.jump( "C Comment", "/\\*", __CommentC);
            }
            
            lang::lexical::scanner &comC = declare("C Comment");
            comC.make( "BLANKS", "[ \t]",    & comC.discard );
            comC.make( "ENDL",   "[:endl:]", & comC.newline );
            comC.make( "ANY1",   ".",        & comC.discard );
            
        }
        
        virtual ~MyLex() throw() {}
        
        void OnCommentC( const regex::token & ) throw()
        {
            std::cerr << "In C Comment" << std::endl;
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyLex);
    };
    
}

YOCTO_UNIT_TEST_IMPL(lexer)
{
    MyLex Lx;
    
    ios::icstream fp( ios::cstdin );
    regex::source src( fp );
    
    
    
    
}
YOCTO_UNIT_TEST_DONE()


