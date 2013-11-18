#include "yocto/utest/run.hpp"
#include "yocto/aqueous/lua.hpp"

#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/ios/ocstream.hpp"

using namespace yocto;
using namespace aqueous;

using namespace aqueous;

YOCTO_UNIT_TEST_IMPL(dosage)
{
    

    library     lib;
    chemsys     cs(lib,1e-7);
    initializer ini(lib);
    
    
    Lua::State VM;
    lua_State *L = VM();
    
    if( argc > 1 )
    {
        Lua::Config::DoFile(L, argv[1]);
        _lua::load( L, lib, "species" );
        _lua::load( L, cs,  "weak"    );
        _lua::load( L, ini, "init"    );
        
        ini.electroneutrality();
        
            
        cs.build();
        ini(cs,0.0);
        
        solution s(lib);
        s.get(cs.C);
        
        ios::ocstream fp( "pH.dat", false );
        
        const double V0       = 10;
        double       V        = V0;
        const double Cbase    = 1e-3;
        const double dV       = 1;
        const double nbase    = Cbase * dV;
        
        fp("%g %g\n", 0.0, s.pH() );
        size_t iter = 0;
        while( s.pH() <= 10.5 )
        {
            ++iter;
            s.mul( V );
            s["HO-"] += nbase;
            s["Na+"] += nbase;
            const double Vbase = iter * dV;
            V = V0 + Vbase;
            s.mul( 1.0 / V );
            s.put(cs.C);
            cs.normalize(0.0);
            s.get(cs.C);
            std::cerr << "Vbase=" << Vbase << ", pH=" << s.pH() << std::endl;
            fp("%g %g\n", Vbase, s.pH() );
        }
        
        
        
    }

}
YOCTO_UNIT_TEST_DONE()

