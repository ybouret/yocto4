#include "yocto/pack/dsf.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;
using namespace pack;

YOCTO_UNIT_TEST_IMPL(dsf)
{
    std::cerr << "sizeof(DSF::Item)=" << sizeof(DSF::Item) << std::endl;

    DSF::Alphabet alpha;
    std::cerr << "INITIAL" << std::endl;
    alpha.display();

    ios::icstream fp( ios::cstdin );
    char C = 0;
    while( fp.query(C) )
    {
        alpha.update(C);
    }


    std::cerr << "BUILDING TREE" << std::endl;
    DSF::Tree tree;
    tree.build_using(alpha);
    tree.graphviz("dsf.dot");
    ios::graphviz_render("dsf.dot",false);

    alpha.display();

}
YOCTO_UNIT_TEST_DONE()
