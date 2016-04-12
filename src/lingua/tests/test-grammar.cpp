#include "yocto/lingua/syntax/grammar.hpp"
#include "yocto/lingua/syntax/term.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;
using namespace lingua;


namespace
{
    class my_grammar : public syntax::grammar
    {
    public:
        typedef syntax::rule Rule;

        my_grammar() : syntax::grammar("my_grammar")
        {

            Rule &INT     = decl_term("INT");
            Rule &OPT_INT = opt(INT);

            top_level(OPT_INT);
            graphviz("g.dot");
            ios::graphviz_render("g.dot",false);

        }

        virtual ~my_grammar() throw()
        {
        }


    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(my_grammar);
    };
}

YOCTO_UNIT_TEST_IMPL(grammar)
{
    std::cerr << "sizeof(xnode)=" << sizeof(syntax::xnode) << std::endl;
    std::cerr << "sizeof(xlist)=" << sizeof(syntax::xlist) << std::endl;

    {
        const string label = "term";
        syntax::terminal tt(label);
        token        data  = "hello";
        lexeme      *lxm   = new lexeme(label,1,data);

        auto_ptr<syntax::xnode> pTerm( syntax::xnode::create(tt,lxm) );
    }

    my_grammar G;


}
YOCTO_UNIT_TEST_DONE()
