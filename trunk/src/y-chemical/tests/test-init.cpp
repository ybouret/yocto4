#include "yocto/utest/run.hpp"
#include "yocto/chemical/initializer.hpp"


using namespace yocto;


YOCTO_UNIT_TEST_IMPL(init)
{
    chemical::collection lib;
    lib.add("H+");
    lib.add("HO-");
    lib.add("AH",0);
    lib.add("A-",-1);
    
    chemical::equilibria cs;
    
    cs.add_water(lib,1e-14);
    cs.add_acid(lib, "acid", "AH", "A-", pow(10,-4.8));
    
    chemical::initializer ini;
    
    
    ini.electroneutrality(lib);
    {
        chemical::constraint &mass = ini.create(0.01);
        mass["AH"] = 1;
        mass["A-"] = 1;
    }
    
    std::cerr << ini << std::endl;
    
    ini(cs,lib,0.0);
    
}
YOCTO_UNIT_TEST_DONE()

