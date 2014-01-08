#include "yocto/utest/run.hpp"
#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/string/conv.hpp"

using namespace yocto;
using namespace math;

namespace {
    static inline void simple_boot()
    {
        chemical::boot::loader ini;
        
        chemical::collection lib;
        chemical::equilibria cs;
        
        lib.add("H+");
        lib.add("HO-");
        lib.add("AcH",0);
        lib.add("Ac-",-1);
        lib.add("Na+",1);
        
        std::cerr << lib << std::endl;
        
        {
            chemical::boot::constraint &ph = ini.add( 1e-6 );
            ph.weight( lib["H+"], 1);
        }
        
        {
            ini.define( lib["Na+"], 0);
        }
        
        ini.electroneutrality(lib);
        
        std::cerr << ini << std::endl;
        
        const size_t Nc = ini.size();
        const size_t M  = lib.size();
        matrix<double> P(Nc,M);
        vector<double> Lam(Nc,0);
        
        lib.update_indices();
        
        ini.fill(P,Lam);
        std::cerr << "P=" << P << std::endl;
        std::cerr << "Lam=" << Lam << std::endl;
        
        std::cerr << "going to solve" << std::endl;
        
        ini.release();
        
        cs.add_water(lib, 1e-14);
        cs.add_acid(lib, "Ac", "AcH", "Ac-", pow(10,-4.78));
        
        std::cerr << cs << std::endl;
        
        ini.electroneutrality(lib);
        ini.define( lib["Na+"], 0);
        const double C0 = 1e-5;
        ini.conserve( lib["AcH"], lib["Ac-"], C0);
        
        std::cerr << ini << std::endl;
        
        ini(cs,lib,0.0);
        
        
    }
}

YOCTO_UNIT_TEST_IMPL(boot)
{
    std::cerr << "sizeof(boot:constituent)=" << sizeof(chemical::boot::constituent) << std::endl;
    std::cerr << "sizeof(boot:constraint)="  << sizeof(chemical::boot::constraint) << std::endl;
    std::cerr << "sizeof(boot:loader)="       << sizeof(chemical::boot::loader) << std::endl;
    
    std::cerr << "testing simple" << std::endl;
    simple_boot();
    std::cerr << std::endl;
    
    
    if( argc > 1)
    {
        Lua::State VM;
        lua_State *L = VM();
        Lua::Config::DoFile(L,argv[1]);
        
        chemical::collection lib;
        chemical::_lua::load(L, lib, "species");
        std::cerr << lib << std::endl;
        
        chemical::equilibria cs;
        chemical::_lua::load(L, lib, cs, "eqs");
        
        chemical::boot::loader ini;
        ini.electroneutrality(lib);
        chemical::_lua::load(L, ini, "ini", lib);
        std::cerr << ini << std::endl;
        
        ini(cs,lib,0.0);
        chemical::solution S(lib);
        S.load(cs.C);
        std::cerr << S << std::endl;
        
    }
    
    
}
YOCTO_UNIT_TEST_DONE()

YOCTO_UNIT_TEST_IMPL(boot1)
{
    double C0 = 1e-4;
    if( argc > 1 )
    {
        C0 = strconv::to_double(argv[1],"C0");
    }
    
    chemical::collection lib;
    lib.add("H+");
    lib.add("HO-");
    lib.add("Na+");
    lib.add("Cl-");
    lib.add("AcH",0);
    lib.add("Ac-",-1);
    
    chemical::equilibria cs;
    cs.add_water(lib, 1e-14);
    cs.add_acid(lib, "Ac", "AcH", "Ac-", pow(10,-4.78) );
    
    chemical::boot::loader ini;
    ini.electroneutrality(lib);
    ini.conserve( lib["AcH"], lib["Ac-"], C0);
    ini.define( lib["Cl-"], 0.001 );
    ini.define( lib["Na+"], 0.001 );
    
    std::cerr << lib << std::endl;
    std::cerr << cs  << std::endl;
    std::cerr << ini << std::endl;
    
    ini(cs,lib,0);
    
    chemical::solution S(lib);
    S.load( cs.C );
    std::cerr << "S=" << S << std::endl;
    std::cerr << "pH=" << S.pH() << std::endl;
    
    
    }
    YOCTO_UNIT_TEST_DONE()
    
    YOCTO_UNIT_TEST_IMPL(boot2)
    {
        double C0 = 1e-4;
        if( argc > 1 )
        {
            C0 = strconv::to_double(argv[1],"C0");
        }
        
        chemical::collection lib;
        lib.add("H+");
        lib.add("HO-");
        lib.add("CO2");
        lib.add("HCO3-");
        lib.add("CO3--");
        
        
        chemical::equilibria cs;
        cs.add_water(lib,1e-14);
        cs.add_acid(lib, "eq1", "CO2",   "HCO3-", 4.16e-7);
        cs.add_acid(lib, "eq2", "HCO3-", "CO3--", 4.69e-11);
        
        std::cerr << cs << std::endl;
        
        chemical::boot::loader ini;
        ini.electroneutrality(lib);
        ini.conserve( lib["CO2"], lib["HCO3-"], lib["CO3--"],C0);
        
        ini(cs,lib,0);
        
        chemical::solution S(lib);
        S.load( cs.C );
        std::cerr << "S=" << S << std::endl;
        std::cerr << "pH=" << S.pH() << std::endl;
        
        
        
    }
    YOCTO_UNIT_TEST_DONE()
    
    
    YOCTO_UNIT_TEST_IMPL(bootH)
    {
        double C0 = 1e-4;
        if( argc > 1 )
        {
            C0 = strconv::to_double(argv[1],"C0");
        }
        
        chemical::collection lib;
        
        lib.add("H+");
        lib.add("HO-");
        lib.add("Na+");
        lib.add("Cl-");
        lib.add("AcH", 0);
        lib.add("Ac-",-1);
        
        
        chemical::equilibria cs;
        cs.add_water(lib, 1e-14);
        cs.add_acid(lib, "Ac", "AcH", "Ac-", pow(10,-4.78) );
        
        chemical::boot::loader ini;
        ini.electroneutrality(lib);
        ini.conserve( lib["AcH"], lib["Ac-"], C0);
        ini.define( lib["Cl-"], 5e-4 );
        ini.define( lib["H+"],  pow(10,-7.0) );
        
        std::cerr << lib << std::endl;
        std::cerr << cs  << std::endl;
        std::cerr << ini << std::endl;
        
        ini(cs,lib,0);

        
    }
    YOCTO_UNIT_TEST_DONE()
    
