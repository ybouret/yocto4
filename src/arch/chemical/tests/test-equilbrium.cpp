#include "yocto/chemical/equilibrium.hpp"
#include "yocto/utest/run.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(equilibrium)
{
    collection lib;
    lib.add("H+",1);
    lib.add("HO-",-1);
    
    const_equilibrium Eq("water",1e-14);
    Eq.add( lib["H+"], 1);
    Eq.add( lib["HO-"], 1);
    
    
    std::cerr << Eq << std::endl;
    
    Eq.add( lib["H+"], 1);
    std::cerr << Eq << std::endl;
   
    Eq.add( lib["H+"], -2);
    std::cerr << Eq << std::endl;

    Eq.add( lib["H+"], 1);
    std::cerr << Eq << std::endl;


}
YOCTO_UNIT_TEST_DONE()
