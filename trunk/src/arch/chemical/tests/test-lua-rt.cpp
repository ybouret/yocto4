#include "yocto/chemical/lua/io.hpp"
#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/chemical/solution.hpp"

#include "yocto/lua/lua.hpp"

using namespace yocto;




YOCTO_UNIT_TEST_IMPL(lua_rt)
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
    
    {
        const char code[] =
        "function show_solution(s)\n"
        "\tfor k in pairs(s) do\n"
        "\t\tprint( \"s[\" .. k .. \"]=\" .. s[k] );\n"
        "\tend\n"
        "end\n";
        std::cerr << code << std::endl;
        Lua::Config::DoString(L,code);
    }
    
    lua_getglobal(L,"show_solution");
    assert( lua_isfunction(L,-1) );
    chemical::_lua::push_solution(L,s);
    if( lua_pcall(L,1,0,0) )
        throw exception( "%s", lua_tostring(L,-1) );
    
    
}
YOCTO_UNIT_TEST_DONE()
