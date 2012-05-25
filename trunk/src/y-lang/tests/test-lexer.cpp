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
            
            const lang::lexical::callback EnterCommentC( this, & MyLex::OnEnterCommentC);
            const lang::lexical::callback LeaveCommentC( this, & MyLex::OnLeaveCommentC);

            scan.jump( "C Comment", "/\\*", EnterCommentC);
            
            lang::lexical::scanner &comC = declare("C Comment");
            comC.make( "BLANKS", "[ \t]",    & comC.discard );
            comC.jump( "MyLex", "\\*/", LeaveCommentC);
            comC.make( "ENDL",   "[:endl:]", & comC.newline );
            comC.make( "ANY1",   "." );
            
            
        }
        
        virtual ~MyLex() throw() {}
        
        void OnEnterCommentC( const regex::token & ) throw()
        {
            std::cerr << "<C Comment>" << std::endl;
        }
        
        void OnLeaveCommentC( const regex::token & ) throw()
        {
            std::cerr << "</C Comment>" << std::endl;
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
    
    lang::lexeme *lx = NULL;
    lang::lexemes lxs;
    while( NULL != ( lx = Lx.next_lexeme(src) ) )
    {
        std::cerr << "line: " << Lx.line << ": " << lx->label << " [" << *lx << "]" << std::endl;
        lxs.push_back(lx);
    }
    
    
}
YOCTO_UNIT_TEST_DONE()


