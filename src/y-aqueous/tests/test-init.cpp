#include "yocto/utest/run.hpp"
#include "yocto/aqueous/initializer.hpp"
#include "yocto/math/types.hpp"

using namespace yocto;
using namespace aqueous;

YOCTO_UNIT_TEST_IMPL(init)
{
    
    
    library lib;
    chemsys cs(lib,1e-4);
    
    lib.add( "H+", 1 );
    lib.add( "HO-", -1 );
    lib.add( "AcH", 0 );
    lib.add( "Ac-", -1);
    
    
    equilibrium &water = cs.create( "water", 1e-14 );    
    water.add("H+", 1);
    water.add("HO-",1);
    std::cerr << water << std::endl;
    
    
    
    equilibrium &Ac = cs.create("Ac", pow(10,-4.8));
    Ac.add("H+",1);
    Ac.add("Ac-",1);
    Ac.add("AcH",-1);
    std::cerr << Ac << std::endl;
    
    
    cs.build();
    std::cerr << "nu=" << cs.nu << std::endl;
    std::cerr << "nuR=" << cs.nuR << std::endl;
    std::cerr << "nuP=" << cs.nuP << std::endl;

}
YOCTO_UNIT_TEST_DONE()
