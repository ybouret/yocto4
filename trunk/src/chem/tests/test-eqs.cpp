#include "yocto/utest/run.hpp"
#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(eqs)
{
    library    lib;
    equilibria eqs;
    
    Lua::State VM;
    lua_State *L = VM();
    
    
    if(argc>1)
    {
        Lua::Config::DoFile(L,argv[1]);
    }
    
    
    __lua::load(L, lib, "species");
    std::cerr << "lib=" << lib << std::endl;
    
    __lua::load(L,eqs,"eqs", lib);
    std::cerr << eqs << std::endl;
}
YOCTO_UNIT_TEST_DONE()
