#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

using namespace yocto;
using namespace chemical;


YOCTO_UNIT_TEST_IMPL(eq)
{
    library lib;
    
    lib.add("H2O",0);
    lib.add("H+", 1);
    lib.add("HO-", -1);
    
    const_equilibrium Eq("water",1e-14);
    Eq.add( lib["H+"], 1);
    Eq.add( lib["HO-"], 1);
    Eq.add( lib["H2O"], -1);

    std::cerr << Eq << std::endl;

    Eq.add( lib["H+"], 1);
    std::cerr << Eq << std::endl;
    Eq.add( lib["H2O"], 1);
    std::cerr << Eq << std::endl;
    
    
}
YOCTO_UNIT_TEST_DONE()
