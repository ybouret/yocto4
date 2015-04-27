#include "yocto/lang/lexer.hpp"
#include "yocto/lang/syntax/xnode.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ptr/auto.hpp"

#include "yocto/lang/lexical/plugin/comment.hpp"
#include "yocto/lang/lexical/plugin/ccomment.hpp"
#include "yocto/lang/lexical/plugin/cstring.hpp"

#include "yocto/ios/icstream.hpp"
#include "yocto/lang/syntax/grammar.hpp"

using namespace yocto;
using namespace lang;

namespace{


    class Lexer : public lexer
    {
    public:
        virtual ~Lexer() throw() {}

        inline Lexer() : lexer("xLexer","root")
        {

            lexical::scanner &root = get_root();

            root.emit("ID",  "[_[:alpha:]][:word:]*");
            root.emit("INT", "[:digit:]+");
            root.emit(";", ";");

            lexical::plugin &cstring =load<lexical::cstring>("STR");
            cstring.hook(root);

            lexical::plugin &com     = load<lexical::comment>("COM","//");
            com.hook(root);

            root.drop("WS", "[:blank:]");
            root.endl("ENDL");
            root.emit("SNGL", ".");

        }



    private:
        YOCTO_DISABLE_COPY_AND_ASSIGN(Lexer);
    };

}

YOCTO_UNIT_TEST_IMPL(xnode)
{
    Lexer         lxr;
    lang::source  src;
    ios::icstream fp(ios::cstdin);

    syntax::grammar G("grammar");

    syntax::aggregate &rec = G.agg("REC");

    syntax::alternate &top = G.alt();
    top << G.term("ID") << G.term("INT");

    rec << top;
    rec << G.term(";");

    auto_ptr<syntax::xnode> tree( G.accept(lxr, src, fp) );
    if(tree.is_valid())
    {
        tree->graphivz("xnode.dot");
    }

}
YOCTO_UNIT_TEST_DONE()
