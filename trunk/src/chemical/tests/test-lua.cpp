#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/chemical/solution.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(lua)
{
    Lua::State VM;
    lua_State *L = VM();
    
    if( argc >1 )
    {
        const string filename = argv[1];
        Lua::Config::DoFile(L, filename);
    }
    
    chemical::collection lib;
    chemical:: _lua:: load(L, lib, "species");
    
    std::cerr <<  lib << std::endl;
    
    chemical::equilibria cs;
    chemical:: _lua::load(L, lib, cs,"eqs");
    
    std::cerr  << cs << std::endl;
    
    chemical::boot::loader ini;
    chemical:: _lua::load(L,ini,"ini",lib);
    ini.electroneutrality(lib);

    std::cerr << ini << std::endl;
    
    ini(cs,lib,0);
    
    chemical::solution s(lib);
    
    s.load(cs.C);
    std::cerr << s << std::endl;
    
    chemical::effectors eff;
    chemical::_lua::load(L, eff, "effectors");
    
    chemical::solution ds(s);
    
    chemical::solution S_out(lib);
    
    {
        chemical::effector::ptr *mct = eff.search("MCT");
        if(mct) (**mct).factor = 0.5;
    }
    
    
    eff.append(ds, 0, 0, s, S_out);
    std::cerr << "ds=" << ds << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
