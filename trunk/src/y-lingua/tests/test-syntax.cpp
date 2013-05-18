#include "yocto/utest/run.hpp"
#include "yocto/lingua/grammar.hpp"

using namespace yocto;
using namespace lingua;


namespace
{
    class MyLexer : public lexer
    {
    public:
        explicit MyLexer() : lexer("Lexer")
        {
            lexical::scanner & Main = declare("Main");
            
            Y_LEX_FORWARD(Main, "ID", "[:word:]+");
            Y_LEX_DISCARD(Main, "BLANK", "[:blank:]");
            Y_LEX_DISCARD(Main,"endl", "[:endl:]");
            
        }
        
        virtual ~MyLexer() throw()
        {
            
        }
        
        
    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(MyLexer);
    };
    
}

YOCTO_UNIT_TEST_IMPL(syntax)
{
    std::cerr << "sizeof(xnode)=" << sizeof(syntax::xnode) << std::endl;
    MyLexer Lexer;
    source  Source;
    
    Source.attach_stdin();
    
    grammar G("MyGrammar");
    G.term("ID");
    
    auto_ptr<syntax::xnode> tree( G.parse(Lexer,Source) );
    
    tree->graphviz("tree.dot");
    system("dot -Tpng -otree.png tree.dot");
    
    
}
YOCTO_UNIT_TEST_DONE()
