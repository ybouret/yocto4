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

    std::cerr << "N=" << cs.size() << std::endl;
    std::cerr << "M=" << lib.size() << std::endl;
    
    initializer ini(lib);
    {
        //-- electroneutrality
        constraint &en = ini.create(0.0);
        for( size_t j=1; j <= lib.size(); ++j )
        {
            const species &sp = *lib(j);
            en.add(sp.name, sp.z);
        }
    }
    
    {
        //-- initial acid
        constraint &ac = ini.create(0.00);
        ac.add( "Ac-", 1);
        ac.add( "AcH", 1);
    }
    
    ini(cs,0.0);
    
}
YOCTO_UNIT_TEST_DONE()
