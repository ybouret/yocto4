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
    
    std::cerr << cs << std::endl;

    
    chemical::initializer ini;
    
    
    ini.electroneutrality(lib);
    {
        chemical::constraint &mass = ini.equals(C0);
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
    
    chemical::solution S2(S);
    std::cerr << "S2=" << S2 << std::endl;
    assert( S2.has_same_components_than(S));
    
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(init2)
{
    double C0 = 0.01;
    if( argc > 1 )
    {
        C0 = strconv::to<double>(argv[1],"C0");
    }
    
    
    chemical::collection lib;
    lib.add("H+");
    lib.add("HO-");
    lib.add("CO2");
    lib.add("HCO3-");
    lib.add("CO3--");
    
    
    chemical::equilibria cs;
    cs.add_water(lib,1e-14);
    cs.add_acid(lib, "eq1", "CO2",   "HCO3-", 4.16e-7);
    cs.add_acid(lib, "eq2", "HCO3-", "CO3--", 4.69e-11);
    
    std::cerr << cs << std::endl;
    
    chemical::initializer ini;
    
    
    ini.electroneutrality(lib);
    {
        chemical::constraint &mass = ini.equals(C0);
        mass["CO2"]   = 1;
        mass["HCO3-"] = 1;
        mass["CO3--"] = 1;
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

