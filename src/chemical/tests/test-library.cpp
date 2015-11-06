#include "yocto/chemical/library.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(library)
{
    library lib;

    string        line;
    ios::icstream input( ios::cstdin );
    auto_ptr<xnode> sp( lib.parse_species(input) );
    if(sp.is_valid())
    {
        sp->graphviz("spec.dot");
        ios::graphviz_render("spec.dot");
    }


}
YOCTO_UNIT_TEST_DONE()
