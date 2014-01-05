#include "yocto/chemical/equilibrium.hpp"
#include "yocto/utest/run.hpp"


using namespace yocto;
#include <cmath>

void check_eq( const chemical::equilibrium &eq)
{
    if( eq.is_valid() )
        std::cerr << "\t" << eq.name << " is valid." << std::endl;
    else
        std::cerr << "\t " << eq.name << " is NOT VALID!" << std::endl;
}

YOCTO_UNIT_TEST_IMPL(equilibrium)
{
    chemical::collection lib;
    lib.reserve(16);
    
    chemical::species &proton  = lib.add("H+",1);
    chemical::species &hydroxy = lib.add("HO-",-1);
    
    lib.add("AH",0);
    lib.add("A-",-1);
    
    proton.data.allocate(100);
    hydroxy.data.make<double>(3.0);
    
    for( chemical::collection::iterator i = lib.begin(); i != lib.end(); ++i )
    {
        const chemical::species &sp = **i;
        std::cerr << sp << " +#bytes=" << sp.data.size() << std::endl;
    }
    
    {
        chemical::equilibrium::ptr eq( new chemical::constant_equilibrium("water",1e-14) );
        
        eq->add( lib["H+"], 1);
        eq->add( lib["HO-"], 1);
        
        
        std::cerr << *eq << std::endl;
        check_eq(*eq);
    }
    
    {
        chemical::equilibrium::ptr eq( new chemical::constant_equilibrium("acid",pow(10.0,-4.8)) );
        
        eq->add( lib["H+"], 1);
        eq->add( lib["A-"], 1);
        eq->add( lib["AH"], -1);
        
        
        std::cerr << *eq << std::endl;
        check_eq(*eq);
    }
    
    {
        chemical::equilibrium::ptr eq( new chemical::constant_equilibrium("ugly",pow(10.0,-4.8)) );
        eq->add( lib["AH"],-1);
        eq->add( lib["A-"],1);

        std::cerr << *eq << std::endl;
        check_eq(*eq);
    }
    
}
YOCTO_UNIT_TEST_DONE()

