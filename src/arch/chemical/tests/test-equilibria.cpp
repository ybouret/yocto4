#include "yocto/chemical/equilibria.hpp"
#include "yocto/utest/run.hpp"


using namespace yocto;
#include <cmath>

YOCTO_UNIT_TEST_IMPL(equilibria)
{
    chemical::collection lib;
    lib.reserve(16);
    
    chemical::species &proton  = lib.add("H+",1);
    chemical::species &hydroxy = lib.add("HO-",-1);
    
    lib.add("AH",0);
    lib.add("A-",-1);
    
    lib.add("NH4+");
    lib.add("NH3");
    
    
    proton.data.allocate(100);
    hydroxy.data.make<double>(3.0);
    
    for( chemical::collection::iterator i = lib.begin(); i != lib.end(); ++i )
    {
        const chemical::species &sp = **i;
        std::cerr << sp << " +#bytes=" << sp.data.size() << std::endl;
    }
    
    chemical::equilibria chemsys;
    
    {
        
        chemical::equilibrium &eq = chemsys.add("water", 1e-14);
        eq.add( lib["H+"], 1);
        eq.add( lib["HO-"], 1);
        
    }
    
    {
        chemical::equilibrium &eq = chemsys.add("acid",pow(10.0,-4.8));
        
        eq.add( lib["H+"], 1);
        eq.add( lib["A-"], 1);
        eq.add( lib["AH"], -1);
        
    }
    
    chemsys.add_acid(lib, "amoniac", "NH4+", "NH3", pow(10,-9.2) );
    
    std::cerr << "Equilibria: " << std::endl;
    std::cerr << chemsys << std::endl;
    
    chemsys.build_from(lib);
    
    for(size_t i=1; i <= chemsys.C.size(); ++i )
    {
        chemsys.C[i] = i*1e-5;
    }
    
    chemsys.normalize_C(0);
    
}
YOCTO_UNIT_TEST_DONE()

