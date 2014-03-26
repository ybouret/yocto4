#include "yocto/utest/run.hpp"
#include "yocto/lang/lexer.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;

namespace {
    
    class MyLex : public lang::lexer
    {
    public:
        lang::lexical::scanner &cstr;
        string                  str;
        
        explicit MyLex() : 
        lang::lexer( "main" ), 
        cstr( declare("STRING") ),
        str()
        {
            
            lang::lexical::scanner &scan = first();
            
            scan.make("WORD",    "[:word:]+");
            scan.make("INT",     "[:digit:]+");
            
            scan.call("STRING",  "\"", this, &MyLex::OnEnterString);
            scan.make("BLANKS",  "[ \t]+",    &scan.discard);
            scan.make("NEWLINE", "[:endl:]",  &scan.no_endl);
            
            
            cstr.back("\"", this, & MyLex:: OnLeaveString);
            cstr.make("1CHAR", ".", this, &MyLex::OnChar);
            
        }
        
        virtual ~MyLex() throw()
        {
            
        }
        
        void OnEnterString( const regex::token & )
        {
            std::cerr << "<Enter String>" << std::endl;
            str.clear();
        }
        
        bool OnChar( const regex::token &t )
        {
            assert( 1 == t.size );
            str.append( t.head->data );
            return false; // not a lexeme
        }
        
        void OnLeaveString( const regex::token & )
        {
            std::cerr << "<Leave String>[" << str << "]" << std::endl;
            unget(cstr, str);
        }
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyLex);
    };
    
}

YOCTO_UNIT_TEST_IMPL(string)
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

