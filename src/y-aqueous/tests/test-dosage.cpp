#include "yocto/utest/run.hpp"
#include "yocto/aqueous/lua.hpp"

#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

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
        std::cerr << "Required supplementary init: " << lib.size() - (cs.size()+ini.size()) << std::endl;
        ini(cs,0.0);
        
    }

}
YOCTO_UNIT_TEST_DONE()