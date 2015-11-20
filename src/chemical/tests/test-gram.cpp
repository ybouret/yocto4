#include "yocto/lang/parser.hpp"
#include "yocto/lang/syntax/logic.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/graphviz.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;


static const char *ATOM[] =
{
    "H", "He",
    "Na", "Mg", "Al", "Si", "P", "S", "Cl"
};

class CHEMparser : public lang::parser
{
public:
    explicit CHEMparser() : lang::parser("CHEM","main")
    {


        Alt &atom = alt();
        for(size_t i=0;i<sizeof(ATOM)/sizeof(ATOM[0]);++i)
        {
            atom.append(&text(ATOM[i],lang::syntax::univocal));
        }

        //__________________________________________________________________
        //
        // lexical
        //__________________________________________________________________
        scanner.drop("WS", "[:blank:]");
        scanner.endl("ENDL");

        top_level( zero_or_more(atom) );

        gramviz("chem.dot");
        ios::graphviz_render("chem.dot");

    }

    virtual ~CHEMparser() throw()
    {
    }
    

private:
    YOCTO_DISABLE_COPY_AND_ASSIGN(CHEMparser);
};

YOCTO_UNIT_TEST_IMPL(gram)
{
    CHEMparser prs;
    ios::icstream fp(ios::cstdin);
    auto_ptr<lang::syntax::xnode> tree( prs.run(fp) );
    if(tree.is_valid())
    {
        tree->graphviz("xnode.dot");
        ios::graphviz_render("xnode.dot");
    }

}
YOCTO_UNIT_TEST_DONE()
