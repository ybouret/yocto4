#include "yocto/associative/dualmap.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/string.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(dualmap)
{
    dualmap<int, string, float> dm;
    
    const string s = "toto";
    
    (void) dm.get_key_of(s);
    (void) dm.get_sub_of(10);
    
    dualmap<int, string, float> dm2(10,as_capacity);
    
    dm2.insert(10, s, 1.0f);
    //dm.insert(11, s, 2.0f);

}
YOCTO_UNIT_TEST_DONE()
