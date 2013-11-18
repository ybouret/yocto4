#include "yocto/utest/run.hpp"
#include "yocto/aqueous/initializer.hpp"
#include "yocto/math/types.hpp"
#include <cstdlib>

using namespace yocto;
using namespace aqueous;


YOCTO_UNIT_TEST_IMPL(init)
{
    
    double initAc = 0;
    if( argc > 1 )
        initAc = fabs(strtod(argv[1], NULL));
    
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
    std::cerr << "nu="  << cs.nu  << std::endl;
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
        constraint &ac = ini.create(initAc);
        ac.add( "Ac-", 1);
        ac.add( "AcH", 1);
        ac.set_value(0.000);
    }
    
    std::cerr << "Constraints: " << std::endl;
    std::cerr << ini << std::endl;
    
    ini(cs,0.0);
    solution s(lib);
    s.get(cs.C);
    std::cerr << s << std::endl;
    
    
}
YOCTO_UNIT_TEST_DONE()


YOCTO_UNIT_TEST_IMPL(init2)
{
    
    
    library lib;
    chemsys cs(lib,1e-7);
    
    
    lib.add( "H+",   1 );
    lib.add( "HO-", -1 );
    lib.add( "AcH",  0 );
    lib.add( "Ac-", -1 );
    lib.add( "H2A",  0 );
    lib.add( "HA-", -1 );
    lib.add( "A2-", -2 );
    lib.add( "Na+",  1 );
    
    equilibrium &water = cs.create( "water", 1e-14 );    
    water.add("H+", 1);
    water.add("HO-",1);
    std::cerr << water << std::endl;
    
    
    
    equilibrium &Ac = cs.create("Ac", pow(10,-4.8));
    Ac.add("H+",1);
    Ac.add("Ac-",1);
    Ac.add("AcH",-1);
    std::cerr << Ac << std::endl;
    
    
    equilibrium &K1 = cs.create( "K1", pow(10,-5.2) );
    K1.add("H+",1);
    K1.add("HA-",1);
    K1.add("H2A",-1);
    std::cerr << K1 << std::endl;
    
    equilibrium &K2 = cs.create( "K2", pow(10,-7.2) );
    K2.add("H+",  1);
    K2.add("A2-", 1);
    K2.add("HA-",-1);
    std::cerr << K2 << std::endl;
    
    cs.build();
    std::cerr << "nu="  << cs.nu  << std::endl;
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
        //-- initial acid Ac
        constraint &ac = ini.create(0.00);
        ac.add( "Ac-", 1);
        ac.add( "AcH", 1);
    }

    {
        //-- sodium
        constraint &na = ini.create(0.00);
        na.add( "Na+", 1 );
    }
    
    {
        //-- initial second acid
        constraint &a2 = ini.create(0.001);
        a2.add( "H2A", 1 );
        a2.add( "HA-", 1 );
        a2.add( "A2-", 1 );
    }
    
    std::cerr << "Constraints=" << std::endl;
    std::cerr << ini << std::endl;
    
    ini(cs,0.0);
    
    solution s(lib);
    s.get(cs.C);
    std::cerr << s << std::endl;
    
    //ini.run(cs,0.0);
}
YOCTO_UNIT_TEST_DONE()
