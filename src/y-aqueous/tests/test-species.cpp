#include "yocto/utest/run.hpp"
#include "yocto/aqueous/species.hpp"

using namespace yocto;
using namespace aqueous;

YOCTO_UNIT_TEST_IMPL(species)
{
    species h("H+",1,100);
    species w("HO-",-1);
    
}
YOCTO_UNIT_TEST_DONE()
