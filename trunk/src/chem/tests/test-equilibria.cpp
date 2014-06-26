#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(equilibria)
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
    
    cs.startup(lib);
    solution S(lib);
    cs.computeGamma(0,S);
    std::cerr << "S=" << S << std::endl;
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
}
YOCTO_UNIT_TEST_DONE()
