#include "yocto/utest/run.hpp"
#include "yocto/chemical/initializer.hpp"
#include "yocto/chemical/solution.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(init)
{
    double C0 = 0.01;
    if( argc > 1 )
    {
        C0 = strconv::to<double>(argv[1],"C0");
    }
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
        chemical::constraint &mass = ini.create(C0);
        mass["AH"] = 1;
        mass["A-"] = 1;
    }
    
    std::cerr << ini << std::endl;
    
    ini(cs,lib,0.0);
    chemical::solution S(lib);
    S.load(cs.C);
    
    std::cerr << "C=" << cs.C << std::endl;
    std::cerr << "S=" << S << std::endl;
    std::cerr << "pH=" << S.pH() << std::endl;
}
YOCTO_UNIT_TEST_DONE()

