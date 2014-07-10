#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

using namespace yocto;
using namespace chemical;


YOCTO_UNIT_TEST_IMPL(validate)
{
    collection lib;
    lib.add("H+",   1);
    lib.add("HO-", -1);
    lib.add("AH",   0);
    lib.add("A-",  -1);
    lib.add("Na+", 1);
    
    equilibria cs;
    
    {
        equilibrium::pointer eq( new const_equilibrium("water",1e-14));
        eq->add( lib["H+"], 1);
        eq->add( lib["HO-"], 1);
        if( !cs.insert(eq) )
            throw exception("multiple '%s'", eq->name.c_str());
    }
    
    {
        equilibrium::pointer eq( new const_equilibrium("acid",pow(10.0,-4.7)));
        eq->add( lib["AH"], -1);
        eq->add( lib["H+"], 1);
        eq->add( lib["A-"], 1);
        if( !cs.insert(eq) )
            throw exception("multiple '%s'", eq->name.c_str());
    }
    
    
    std::cerr << cs << std::endl;
    
    cs.startup(lib);
    
    solution S(lib);
    
    S("H+")  = -0.1;
    S("HO-") = 0.05;
    S("AH")  = 0.02;
    //S("A-")  = -0.001;
    
    std::cerr << "S0=" << S << std::endl;
    if(cs.validate(S))
    {
        std::cerr << "S1=" << S << std::endl;
    }
    else
    {
        std::cerr << "unable to validate!" << std::endl;
    }
    
}
YOCTO_UNIT_TEST_DONE()
