#include "yocto/lang/pattern/basic.hpp"
#include "yocto/lang/pattern/joker.hpp"
#include "yocto/lang/pattern/logic.hpp"
#include "yocto/lang/pattern/dict.hpp"
#include "yocto/utest/run.hpp"



using namespace yocto;
using namespace lang;

YOCTO_UNIT_TEST_IMPL(dict)
{
    
    p_dict dict;
    
    dict.add("Z", single::create('Z') );
    pattern *p = dict.create("Z");
    delete p;
    
    
}
YOCTO_UNIT_TEST_DONE()

