#include "yocto/utest/run.hpp"
#include "yocto/lingua/grammar.hpp"
#include "yocto/fs/local-fs.hpp"
#include <cstdlib>

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
            
            Y_LEX_FORWARD(Main, "ALPHA", "[:alpha:]+");
            Y_LEX_FORWARD(Main, "DIGIT", "[:digit:]+");
            Y_LEX_FORWARD(Main, "MODIF", "[@$]");
            Y_LEX_FORWARD(Main, "STOP",  ";" );
            Y_LEX_DISCARD(Main, "BLANK", "[:blank:]");
            Y_LEX_NO_ENDL(Main);
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
    vfs &fs = local_fs::instance();
    if(fs.is_reg("tree.dot" )) fs.remove_file("tree.dot");
    if(fs.is_reg("tree.png"))  fs.remove_file("tree.png");
    
    MyLexer Lexer;
    source  Source;
    
    Source.attach_stdin();
    
    grammar G("MyGrammar");
    syntax::terminal     &ALPHA = G.term("ALPHA");
    syntax::terminal     &DIGIT = G.term("DIGIT");
    syntax::alternative  &ALT1  = G.alt("ALT1");
    ALT1.add(ALPHA);
    ALT1.add(DIGIT);
    syntax::aggregate    &DECL  = G.agg("DECL");
    syntax::terminal     &STOP  = G.term("STOP", syntax::is_specialized);
    syntax::terminal     &MODIF = G.term("MODIF");
    DECL += ALT1;
    DECL += ( G.opt( MODIF ) );
    DECL += STOP;
    
    syntax::repeating &STAT = G.rep( "STAT", DECL, 0);
    
    G.set_root(STAT);
    int depth = 0;
    syntax::xnode *Tree = syntax::xnode::AST( G.accept(Lexer,Source), depth );
    if( Tree )
    {
        auto_ptr<syntax::xnode> tree(Tree);
        tree->graphviz("tree.dot");
        int res=system("dot -Tpng -otree.png tree.dot");(void)res;
    }
    else
        std::cerr << "Accepted Empty Tree" << std::endl;
    
    
    
}
YOCTO_UNIT_TEST_DONE()
