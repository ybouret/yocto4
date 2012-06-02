#include "yocto/utest/run.hpp"
#include "yocto/aqueous/chemsys.hpp"
#include "yocto/math/types.hpp"

using namespace yocto;
using namespace aqueous;

YOCTO_UNIT_TEST_IMPL(chemsys)
{
    
    
    library lib;
    chemsys cs(lib);
    
    lib.add( "H+", 1 );
    lib.add( "HO-", -1 );
    lib.add( "AcH", 0 );
    lib.add( "Ac-", -1);
    
    
        equilibrium &water = cs.create( "water", 1e-14 );    
        water.add("H+", 1);
        water.add("HO-",1);
        std::cerr << water << std::endl;
    
    
    {
        equilibrium &Ac = cs.create("Ac", pow(10,-4.8));
        Ac.add("H+",1);
        Ac.add("Ac-",1);
        Ac.add("AcH",-1);
        std::cerr << Ac << std::endl;
    }
    
    cs.build();
    std::cerr << "nu=" << cs.nu << std::endl;
    std::cerr << "nuR=" << cs.nuR << std::endl;
    std::cerr << "nuP=" << cs.nuP << std::endl;
    
    solution s(lib);
    s["H+"] = 1e-6;
    s["HO-"] = 1.1 * water.K(0) / s["H+"];
    cs.computeGamma(s,0.0);
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    std::cerr << "water: " << water.Gamma(s, 0.0) << std::endl;
}
YOCTO_UNIT_TEST_DONE()
