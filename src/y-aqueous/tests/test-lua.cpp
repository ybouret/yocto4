#include "yocto/utest/run.hpp"
#include "yocto/aqueous/lua.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

using namespace yocto;
using namespace aqueous;



YOCTO_UNIT_TEST_IMPL(lua)
{
    
    
    library lib;
    
    lib.add( "H+", 1 );
    lib.add( "HO-", -1 );
    lib.add( "AcH", 0 );
    lib.add( "Ac-", -1);
    
    solution s(lib);
    
    Lua::State VM;
    lua_State *L = VM();
    
    {
        const string code = 
        "function display(S)\n"
        "print( \"#items=\" .. table.getn(S) );\n"
        "for i,line in ipairs(S) do\n"
        "    print(line);\n"
        "end\n"
        "end\n";
        Lua::Config::DoString(L,code);
    }
    
    lua_settop(L,0);
    lua_getglobal(L, "display");
    if( !lua_isfunction(L, -1) )
        throw exception("Invalid Lua Function");
    _lua::push(L,s);
    
    
    std::cerr <<"#=" << lua_objlen(L, -1) << std::endl;
    if( lua_pcall(L,1,0,0) ) throw exception( "%s", lua_tostring(L,-1) );
}
YOCTO_UNIT_TEST_DONE()
