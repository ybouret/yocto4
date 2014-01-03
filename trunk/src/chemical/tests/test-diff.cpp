#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/chemical/solution.hpp"
#include "yocto/ios/ocstream.hpp"
#include "yocto/fs/vfs.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(diff)
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
    
    chemical::initializer ini;
    chemical:: _lua::load(L,ini,"ini");
    ini.electroneutrality(lib);
    
    std::cerr << ini << std::endl;
    
    ini(cs,lib,0);
    
    chemical::solution s(lib);
    
    s.load(cs.C);
    std::cerr << s << std::endl;
    std::cerr << "pH=" << s.pH() << std::endl;
    
    chemical::effectors eff;
    chemical::_lua::load(L, eff, "effectors");
    std::cerr << "Effectors: " << std::endl << eff << std::endl;
    chemical::solution ds(s);
    
    eff.collect(ds, 0, 0, s);
    std::cerr << "ds=" << ds << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
