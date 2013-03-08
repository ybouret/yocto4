#include "yocto/utest/run.hpp"
#include "yocto/spade/visit-silo.hpp"

using namespace yocto;
using namespace spade;


YOCTO_UNIT_TEST_IMPL(silo)
{
    Silo::File db( "demo.silo", "Just A Demo");
    
}
YOCTO_UNIT_TEST_DONE()
