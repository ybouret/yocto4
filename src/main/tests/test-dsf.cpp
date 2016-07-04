#include "yocto/pack/dsf.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace pack;

YOCTO_UNIT_TEST_IMPL(dsf)
{
    std::cerr << "sizeof(DSF::Alphabet::Item)=" << sizeof(DSF::Alphabet::Item) << std::endl;

    DSF::Alphabet alpha;
    alpha.display();

}
YOCTO_UNIT_TEST_DONE()
