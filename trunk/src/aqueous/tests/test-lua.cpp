#include "yocto/utest/run.hpp"
#include "yocto/aqueous/lua.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

using namespace yocto;
using namespace aqueous;



YOCTO_UNIT_TEST_IMPL(lua)
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
        
        std::cerr << "Starting With" << std::endl;
        std::cerr << s << std::endl;
        
        _lua::effectors  edb;
        _lua::load(L,edb,"effectors");
        
        solution ds(lib);
        ds.ldz();
        
        _lua::effector::ptr *ppEff = edb.query("MCT");
        if( !ppEff )
            throw exception("Missing MCT");
        
        _lua::effector &MCT = **ppEff;
        MCT.call(L, ds, 1.0, 0.0, s);
        std::cerr << "ds=" << std::endl << ds << std::endl;
    }
    
    
}
YOCTO_UNIT_TEST_DONE()
