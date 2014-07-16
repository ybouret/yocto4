#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

using namespace yocto;
using namespace chemical;


static inline
double my_round(double x)
{
    return floor(x+0.5);
}

YOCTO_UNIT_TEST_IMPL(validate)
{
    collection lib;
    
    bool add_extra = true;
    
    lib.add("H+",   1);
    lib.add("HO-", -1);
    lib.add("AH",   0);
    lib.add("A-",  -1);
    //lib.add("Na+", 1);
    
    if(add_extra)
    {
        lib.add("XH2",0);
        lib.add("X--",-2);
    }
    
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
    
    if(add_extra)
    {
        equilibrium::pointer eq( new const_equilibrium("acid2",pow(10.0,-6.4)));
        eq->add( lib["XH2"], -1);
        eq->add( lib["H+"],   2);
        eq->add( lib["X--"],  1);
        if( !cs.insert(eq) )
            throw exception("multiple '%s'", eq->name.c_str());

    }
    
    
    std::cerr << cs << std::endl;
    
    cs.startup(lib);
    
    solution S(lib);
    
    S("H+")  = -0.1;
    S("HO-") = 0.05;
    S("AH")  = 0.02;
    S("A-")  = -0.001;
    
    std::cerr << "S0=" << S << std::endl;
    if(cs.validate(S))
    {
        std::cerr << "S1=" << S << std::endl;
    }
    else
    {
        std::cerr << "unable to validate!" << std::endl;
    }
    
    
    const double xx[] = { 7.99, -7.99, 7.01, -7.01 };
    for(size_t i=0; i < sizeof(xx)/sizeof(xx[0]);++i )
    {
        std::cerr << "round(" << xx[i] << ")=" << my_round(xx[i]) << std::endl;
    }

    
}
YOCTO_UNIT_TEST_DONE()
