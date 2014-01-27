#include "yocto/utest/run.hpp"
#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"


using namespace yocto;

YOCTO_UNIT_TEST_IMPL(tables)
{
    Lua::State vm;
    lua_State *L = vm();
    
    lua_settop(L, 0);
    
    lua_createtable(L, 0, 5);
    
    
    lua_pushinteger(L, 1); // key
    lua_pushinteger(L, 2); // value
    
    lua_rawset(L, -3);
    
    lua_pushstring(L, "H+");
    lua_pushnumber(L, 1e-4);
    lua_rawset(L,-3);
    
    lua_setglobal(L, "toto");

    
    for( int i=1; i < argc; ++i )
    {
        const char *filename = argv[i];
        Lua::Config::DoFile( L, filename );
    }
    
}
YOCTO_UNIT_TEST_DONE()
