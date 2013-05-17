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
            Y_LEX_DISCARD(Main, "BLANKS", "[:blank:]+");
            Main.make("ENDL", "[:endl:]", this, & Lexer::OnNewline);
            
        }
        
    
        virtual ~Lexer() throw()
        {
        }
    
        bool OnNewline( const token & ) 
        {
            std::cerr << "[<" << current().name << ">ENDL #" << line << "]" << std::endl;
            ++line;
            return false; // no lexeme
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

