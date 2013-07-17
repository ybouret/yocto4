#include "yocto/utest/run.hpp"
#include "yocto/chemical/initializer.hpp"


using namespace yocto;


YOCTO_UNIT_TEST_IMPL(init)
{
    chemical::collection lib;
    lib.add("H+");
    lib.add("HO-");
    lib.add("Na+");
    
    chemical::initializer ini;
    
    chemical::constraint  &C1 = ini.create(1);
    C1["Na+"] = 1;
    
    std::cerr << C1 << std::endl;
    ini.electroneutrality(lib);
    
    std::cerr << ini << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()

