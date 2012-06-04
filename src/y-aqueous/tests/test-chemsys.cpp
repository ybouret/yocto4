#include "yocto/utest/run.hpp"
#include "yocto/aqueous/chemsys.hpp"
#include "yocto/math/types.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace aqueous;

YOCTO_UNIT_TEST_IMPL(chemsys)
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
    
    solution s(lib);
    s["H+"] = 1e-6;
    s["HO-"] = 1.1 * water.K(0) / s["H+"];
    s["AcH"] = 0.1;
    s["Ac-"] = 0.01;
    
    std::cerr << "s=" << std::endl << s << std::endl;
    
    s.put(cs.C);
    std::cerr << "C=" << cs.C << std::endl;
    
    
    cs.computeW(0.0,false);
    std::cerr << "water: " << water.Gamma(s, 0.0) << std::endl;
    std::cerr << "Ac:    " << Ac.Gamma(s,0.0)     << std::endl;
    std::cerr << "Gamma="  << cs.Gamma << std::endl;
    std::cerr << "Phi="    << cs.Phi << std::endl;
    std::cerr << "W="      << cs.W   << std::endl;
    
    std::cerr << "Normalizing..." << std::endl;
    cs.normalize(0.0);
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
    
    std::cerr << "Reducing...." << std::endl;
    for( size_t i=cs.dC.size(); i >0; --i )
        cs.dC[i] = alea<double>();
    
    std::cerr << "C =" << cs.C << std::endl;
    std::cerr << "dC=" << cs.dC << std::endl;
    cs.reduce(0.0);
    std::cerr << "dC=" << cs.dC << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
