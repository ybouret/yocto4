#include "yocto/utest/run.hpp"
#include "yocto/lingua/lexer.hpp"

using namespace yocto;
using namespace lingua;

namespace
{
    class Lexer : public lexer
    {
    public:
        explicit Lexer() : lexer( "MyLexer" )
        {
            lexical::scanner &Main = declare( "Main" );
            
            Y_LEX_FORWARD(Main, "ID",     "[:word:]+" );
            Y_LEX_FORWARD(Main, "INT",    "[:digit:]+");
            Y_LEX_DISCARD(Main, "BLANKS", "[:blank:]+");
            Main.make("ENDL", "[:endl:]", this, & Lexer::OnNewline);
            Main.jump("C++Comment", "//", this, & Lexer::EnterCom1 );

            lexical::scanner &Com1 = declare( "C++Comment" );
            Y_LEX_DISCARD(Com1, "1DOT", ".");
            Com1.jump("Main", "[:endl:]", this, &Lexer::LeaveCom1 );
            
            
        }
        
    
        virtual ~Lexer() throw()
        {
        }
    
        
        void Newline()
        {
            std::cerr << "[<" << current().name << ">ENDL #" << line << "]" << std::endl;
            ++line;
        }
        
        bool OnNewline( const token & )
        {
            Newline();
            return false; // no lexeme
        }
        
        
        
        void EnterCom1( const token & )
        {
            std::cerr << "<C++Comment>" << std::endl;
        }
        
        
        void LeaveCom1( const token & )
        {
            std::cerr << "</C++Comment>" << std::endl;
            Newline();
        }
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Lexer);
        
    };
}


YOCTO_UNIT_TEST_IMPL(lexer)
{
    
    Lexer   Lxx;
    lexemes lxms;
    source  src;
    src.attach_stdin();
    
    lexeme *lx = 0;
    while( 0 != (lx=Lxx.get(src) ) )
    {
        lxms.push_back(lx);
        std::cerr << "[" << lx->label << "]=" << *lx << std::endl;
    }
    
    
    

}
YOCTO_UNIT_TEST_DONE()

