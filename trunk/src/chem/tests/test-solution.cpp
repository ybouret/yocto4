#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

using namespace yocto;
using namespace chemical;

YOCTO_UNIT_TEST_IMPL(solution)
{
    collection lib;
    lib.add("H+",1);
    lib.add("HO-",-1);
    
    solution   S(lib);
    
    
    S[1] = 1;
    S[2] = 3;
    S("H+") = 0.1;
    std::cerr << S << std::endl;
    std::cerr << "pH=" << S.pH() << std::endl;
    
    Lua::State VM;
    lua_State *L = VM();
    
    _lua::push(L,S);
    S.ldz();
    _lua::load(L,S);
    std::cerr << "S_lua=" << S << std::endl;
    
    _lua::push(L,S,lib);
    S.ldz();
    _lua::load(L,S,lib);
    std::cerr << "S_lua=" << S << std::endl;
    
}
YOCTO_UNIT_TEST_DONE()
