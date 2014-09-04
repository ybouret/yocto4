#include "yocto/lang/grammar.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace lang;


namespace
{
    class MyLex : public lexer
    {
    public:
        virtual ~MyLex() throw()
        {
        }
        
        MyLex() : lexer("MyLex")
        {
            lexical::scanner &Main = declare("Main");
            
            
            Main.make("identifier", "[:alpha:][:word:]+",forward);
            Main.make("blank","[:blank:]",discard);
            Main.make("endl", "[:endl:]" ,newline);
            
            dict.release();
        }
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyLex);
    };
}

YOCTO_UNIT_TEST_IMPL(grammar)
{
    
    ios::icstream Input( ios::cstdin );
    source        Source;
    MyLex         Lexer;
    grammar       G("Grammar");
    
    G.term("identifier");

    auto_ptr<syntax::xnode> Tree( G.accept(Lexer, Source, Input) );
    
    
    
    
    
    
}
YOCTO_UNIT_TEST_DONE()
