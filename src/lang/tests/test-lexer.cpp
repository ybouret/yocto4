#include "yocto/lang/lexer.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"


using namespace yocto;
using namespace lang;


namespace  {
    
    class MyLexer : public lexer
    {
    public:
        
        
        MyLexer() : lexer("MyLexer")
        {
            
            lexical::scanner &Main = declare("Main");
            
            Main.make("ID",  "[:alpha:][:word:]*",this, &MyLexer::OnID);
            Main.make("INT", "[:digit:]+",       this, &MyLexer::OnINT);
            Main.jump("Main2", "@2", this, &MyLexer::OnJump);
            Main.call("COM", "//",  this, &MyLexer::OnCall);
            Main.make("BLANK", "[:blank:]",discard);
            Main.make("ENDL", "[:endl:]", this, &MyLexer::OnNewLine);
            
            
            lexical::scanner &Main2 = declare("Main2");
            
            Main2.make("ID",  "[:alpha:][:word:]*",this, &MyLexer::OnID);
            Main2.make("INT", "[:digit:]+",       this, &MyLexer::OnINT);
            Main2.jump("Main", "@1", this, &MyLexer::OnJump);
            Main2.call("COM", "//",  this, &MyLexer::OnCall);
            Main2.make("BLANK", "[:blank:]",discard);
            Main2.make("ENDL", "[:endl:]", &Main, &lexical::scanner::newline);
            
            lexical::scanner &COM = declare("COM");
            COM.make("CONTENT", ".", discard);
            COM.back("[:endl:]", this, &MyLexer::NewLine);
            
        }
        
        virtual ~MyLexer() throw()
        {
        }
        
        
        
    private:
        bool OnID( const token & )
        {
            return true;
        }
        
        bool OnINT( const token & )
        {
            return true;
        }
        
        void NewLine(const token &)
        {
            std::cerr << "#EOL " << line << std::endl;
            ++line;
        }
        
        bool OnNewLine(const token &t)
        {
            NewLine(t);
            return false;
        }
        
        void OnJump( const token &t )
        {
            std::cerr << "Jumping" << t << std::endl;
        }
        
        void OnCall( const token &t )
        {
            //std::cerr << "Calling" << t << std::endl;
        }
    };
    
}


YOCTO_UNIT_TEST_IMPL(lexer)
{
    ios::icstream fp( ios::cstdin );
    source        src;
    
    MyLexer lex;
    lexemes lxm;
    while(true)
    {
        lexeme *lx = lex.get(src, fp);
        if(!lx)
        {
            std::cerr << "EOF" << std::endl;
            break;
        }
        lxm.push_back(lx);
        std::cerr << lx->label << ":" << *lx << std::endl;
        lxm.kill();
    }
}
YOCTO_UNIT_TEST_DONE()
