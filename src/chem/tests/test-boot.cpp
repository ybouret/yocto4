#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/code/rand.hpp"
#include "yocto/chemical/lua/io.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(boot)
{
    collection  lib;
    equilibria  cs;
    Lua::State  VM;
    lua_State  *L = VM();
    
    if(argc>1)
    {
        Lua::Config::DoFile(L, argv[1]);
    }
    
    _lua::load(L,lib,"species",0);
    std::cerr << lib << std::endl;
    
    _lua::load(L,lib,cs,"eqs");
    std::cerr << cs << std::endl;

    boot loader;
    loader.electroneutrality(lib);
    _lua::load(L,lib,loader,"ini");

    std::cerr << loader << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()

