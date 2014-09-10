#include "yocto/utest/run.hpp"
#include "yocto/lingua/lexer.hpp"

using namespace yocto;
using namespace lingua;

namespace
{
    class Lexer : public lexer
    {
    public:
        string str;
        
        explicit Lexer() : lexer( "MyLexer" ), str()
        {
            lexical::scanner &Main = declare( "Main" );
            
            Y_LEX_FORWARD(Main, "ID",     "[:word:]+" );
            Y_LEX_FORWARD(Main, "INT",    "[:digit:]+");
            Main.call("STRING", "\\x22", this, & Lexer::EnterString);
            Y_LEX_FORWARD(Main, "PUNCT",  "[:punct:]" );
            Y_LEX_DISCARD(Main, "BLANKS", "[:blank:]+");
            Main.make("ENDL", "[:endl:]", this, & Lexer::OnNewline);
            Main.jump("C++Comment", "//", this, & Lexer::EnterCom1 );
            Main.call("C Comment", "/\\*\\*", this, & Lexer:: EnterCom2);
            
            lexical::scanner &Com1 = declare( "C++Comment" );
            Y_LEX_DISCARD(Com1, "1DOT", ".");
            Com1.jump("Main", "[:endl:]", this, &Lexer::LeaveCom1 );
            
            lexical::scanner &Com2 = declare( "C Comment" );
            Y_LEX_DISCARD(Com2, "1DOT", ".");
            Com2.make("ENDL", "[:endl:]", this, & Lexer::OnNewline);
            Com2.back("\\*\\*/", this, & Lexer::LeaveCom2);
            
            lexical::scanner &Str = declare("STRING");
            Str.back("\\x22", this, & Lexer::LeaveString);
            Str.make("1REG", ".", this, & Lexer::StringRegular);
            
            
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
        
        void EnterCom2( const token & )
        {
            std::cerr << "<C Comment>" << std::endl;
        }
        
        void LeaveCom2( const token & )
        {
            std::cerr << "</C Comment>" << std::endl;
        }
        
        void EnterString( const token & )
        {
            std::cerr << "<STRING>" << std::endl;
            str.clear();
        }
        
        bool StringRegular( const token &t )
        {
            assert(t.size==1);
            str.append(t.head->data);
            return false;
        }
        
        void LeaveString( const token & )
        {
            std::cerr << "</STRING='" << str << "'>" << std::endl;
            // => Mimicking a string in the calling scanner
            sub("STRING").emit(*this, str);
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
    
    if( Lxx.is_active(src) )
        throw exception("Source should be inactive!");
    
    
    

}
YOCTO_UNIT_TEST_DONE()

