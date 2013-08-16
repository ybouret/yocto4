#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/chemical/solution.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(dosage)
{
    //--------------------------------------------------------------------------
    // use lua to load parameters
    //--------------------------------------------------------------------------
    Lua::State VM;
    lua_State *L = VM();
    
    if( argc >1 )
    {
        const string filename = argv[1];
        Lua::Config::DoFile(L, filename);
    }
    else
        throw exception("need a config file to do something");
    
    //--------------------------------------------------------------------------
    // load the species into collection
    //--------------------------------------------------------------------------
    chemical::collection      lib;
    chemical:: _lua:: load(L, lib, "species");
    std::cerr << lib << std::endl;
    
    //--------------------------------------------------------------------------
    // load the reactions
    //--------------------------------------------------------------------------
    chemical::equilibria cs;
    chemical:: _lua::load(L, lib, cs,"eqs");
    std::cerr  << cs << std::endl;
    
    //--------------------------------------------------------------------------
    // prepare the initializer
    //--------------------------------------------------------------------------
    chemical::initializer ini;
    ini.electroneutrality(lib);
    chemical:: _lua::load(L,ini,"ini");
    
    std::cerr << ini << std::endl;
    
    //--------------------------------------------------------------------------
    // find the initial composition
    //--------------------------------------------------------------------------
    cs.ftol = 1e-7;
    ini(cs,lib,0.0);
    
    chemical::solution S(lib);
    S.load(cs.C);
    std::cerr << "pH=" << S.pH() << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
