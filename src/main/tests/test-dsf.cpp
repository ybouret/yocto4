#include "yocto/pack/dsf.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/ios/icstream.hpp"

using namespace yocto;
using namespace pack;

YOCTO_UNIT_TEST_IMPL(dsf)
{
    std::cerr << "sizeof(DSF::Alphabet::Item)=" << sizeof(DSF::Alphabet::Item) << std::endl;

    DSF::Alphabet alpha;
    alpha.display();

    ios::icstream fp( ios::cstdin );
    char C = 0;
    while( fp.query(C) )
    {
        alpha.update(C);
    }

    alpha.display();
}
YOCTO_UNIT_TEST_DONE()
