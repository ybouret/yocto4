#include "yocto/lang/grammar.hpp"
#include "yocto/utest/run.hpp"

#include "yocto/ios/icstream.hpp"

#include <cstdlib>

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
			Main.make("number","[:digit:]+",forward);
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
    
    syntax::rule &identifier = G.term("identifier");
	syntax::rule &number     = G.term("number");
    syntax::rule &optid      = G.opt(identifier);
    syntax::rule &z_id       = G.zero_or_more("zom_id",identifier);
    syntax::rule &o_id       = G.one_or_more("oom_id",identifier);
    syntax::aggregate &numid = G.agg("numid");
	
	numid << identifier << number;

    if(argc>1)
    {
        const string arg = argv[1];
        if(arg=="?")
            G.set_root(optid);
        
        if(arg=="*")
        {
            G.set_root(z_id);
        }
        
        if(arg=="+")
        {
            G.set_root(o_id);
        }
        
    }
    
    G.show_rules();
    
    auto_ptr<syntax::xnode> Tree( G.accept(Lexer, Source, Input) );
    std::cerr << "((" << G.name << ")) success" << std::endl;
    if(Tree.is_valid())
    {
        Tree->graphviz("xnode.dot");
        system("dot -Tpng -o xnode.png xnode.dot");
    }
    else
    {
        
    }
    
    
    
}
YOCTO_UNIT_TEST_DONE()
