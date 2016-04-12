#include "yocto/lingua/syntax/grammar.hpp"
#include "yocto/lingua/syntax/term.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;
using namespace lingua;


namespace
{

    class my_lexer : public lexer
    {
    public:
        explicit my_lexer() : lexer("my_lexer","main")
        {
            root.emit("INT",   "[:digit:]+");
            root.emit("ID",    "[:word:]+");
            root.emit("EQ",    "=");
            root.emit(";",     ";");
            root.drop("BLANK", "[:blank:]");
            root.endl("endl");
        }

        virtual ~my_lexer() throw()
        {
        }


    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(my_lexer);
    };

    class my_grammar : public syntax::grammar
    {
    public:
        typedef syntax::rule Rule;

        my_grammar() : syntax::grammar("my_grammar")
        {

            Rule & INT = decl_term("INT");
            Rule & ID  = decl_term("ID");
            Rule & EQ  = decl_term("EQ");
            Rule & END = decl_term(";");

            syntax::aggregate &PUSH_INT  = agg("PUSH_INT");
            PUSH_INT << INT << END;

            syntax::aggregate &PUSH_ID   = agg("PUSH_ID");
            PUSH_ID << ID << END;

            syntax::aggregate &ASSIGN    = agg("ASSIGN");
            syntax::alternate &RHS       = alt();
            RHS << ID << INT;
            ASSIGN << ID << EQ << RHS << END;

            syntax::alternate &LINE = alt();
            LINE << PUSH_INT << PUSH_ID << ASSIGN;

            top_level( zero_or_more(LINE) );

            graphviz("g.dot");
            ios::graphviz_render("g.dot");

        }

        virtual ~my_grammar() throw()
        {
        }


    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(my_grammar);
    };
}

#include "yocto/ios/icstream.hpp"
#include "yocto/ptr/auto.hpp"

YOCTO_UNIT_TEST_IMPL(grammar)
{
    std::cerr << "sizeof(xnode)=" << sizeof(syntax::xnode) << std::endl;
    std::cerr << "sizeof(xlist)=" << sizeof(syntax::xlist) << std::endl;


    my_grammar    G;
    my_lexer      L;
    ios::icstream fp( ios::cstdin );
    source        src(fp);

    auto_ptr<syntax::xnode> tree( G.run(L,src) );
    if(tree.is_valid())
    {
        tree->graphviz("tree.dot");
        ios::graphviz_render("tree.dot");
    }


}
YOCTO_UNIT_TEST_DONE()
