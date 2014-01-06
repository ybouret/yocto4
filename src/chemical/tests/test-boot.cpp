#include "yocto/utest/run.hpp"
#include "yocto/chemical/boot.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(boot)
{
    std::cerr << "sizeof(boot:constituent)=" << sizeof(chemical::boot::constituent) << std::endl;
    std::cerr << "sizeof(boot:constraint)="  << sizeof(chemical::boot::constraint) << std::endl;
    std::cerr << "sizeof(boot:loader)="       << sizeof(chemical::boot::loader) << std::endl;

    chemical::boot::loader ini;
    
    chemical::collection lib;
    lib.add("H+");
    lib.add("HO-");
    lib.add("AcH",0);
    lib.add("Ac-",-1);
    lib.add("Na+",1);
    
    std::cerr << lib << std::endl;
    
    {
        chemical::boot::constraint &ph = ini.add( 1e-6 );
        ph.weight( lib["H+"], 1);
    }

    {
        ini.define( lib["Na+"], 0);
    }
    
    ini.electroneutrality(lib);
    
    std::cerr << ini << std::endl;
}
YOCTO_UNIT_TEST_DONE()
