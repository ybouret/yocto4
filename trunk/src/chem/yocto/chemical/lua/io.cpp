#include "yocto/chemical/lua/io.hpp"

namespace yocto
{
    
    namespace chemical
    {
        
        static int Lua_GHK_Psi(lua_State *L)
        {
            const double U = lua_tonumber(L,-1);
            lua_pushnumber(L, GHK_Psi(U) );
            return 1;
        }
        
        void __lua:: declare_GHK_Psi(lua_State *L)
        {
            lua_pushcfunction(L, Lua_GHK_Psi);
            lua_setglobal(L, "Psi");
        }
        
    }
}

