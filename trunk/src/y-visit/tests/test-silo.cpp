#include "yocto/utest/run.hpp"
#include "yocto/spade/visit-silo.hpp"

using namespace yocto;
using namespace spade;


YOCTO_UNIT_TEST_IMPL(silo)
{
    Silo::File    db( "demo.silo", "Just A Demo");
    Silo::OptList optlist(2);
    
    optlist.AddTime(0);
    optlist.AddCycle(0);
    
    
}
YOCTO_UNIT_TEST_DONE()
