#include "yocto/pack/dsf.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/ios/graphviz.hpp"

using namespace yocto;
using namespace pack;

YOCTO_UNIT_TEST_IMPL(dsf)
{
#if 0
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
#endif

    DSF::Encoder Q;
    
    ios::icstream fp( ios::cstdin  );
    ios::ocstream op( ios::cstdout );
    char C = 0;
    size_t inp = 0, out=0;

    while(fp.query(C))
    {
        ++inp;
        Q.write(C);
        while( Q.query(C) )
        {
            op.write(C);
            ++out;
        }
    }

    Q.flush();
    while( Q.query(C) )
    {
        op.write(C);
        ++out;
    }
    std::cerr << "inp=" << inp << ", out=" << out << std::endl;

}
YOCTO_UNIT_TEST_DONE()
