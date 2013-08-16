#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/chemical/solution.hpp"

using namespace yocto;


YOCTO_UNIT_TEST_IMPL(dosage)
{
    Lua::State VM;
    lua_State *L = VM();
    
    if( argc >1 )
    {
        const string filename = argv[1];
        Lua::Config::DoFile(L, filename);
    }
    else
        throw exception("need a config file to do something");
    
    chemical::collection      lib;
    chemical:: _lua:: load(L, lib, "species");

    std::cerr << lib << std::endl;

}
YOCTO_UNIT_TEST_DONE()
