#include "yocto/utest/run.hpp"
#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

using namespace yocto;
using namespace math;

namespace {
    static inline void simple_boot()
    {
        chemical::boot::loader ini;
        
        chemical::collection lib;
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
        
        chemical::boot::loader ini;
        chemical::_lua::load(L, ini, "ini", lib);
        std::cerr << ini << std::endl;
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
