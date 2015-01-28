#include "yocto/chemical/lua/io.hpp"
#include "yocto/math/types.hpp"
#include "yocto/physics/constants.hpp"

namespace yocto
{
    
    namespace chemical
    {
    
        using namespace math;
        
        static int Lua_GHK_Psi(lua_State *L)
        {
            const double U = lua_tonumber(L,-1);
            lua_pushnumber(L, GHK_Psi(U) );
            return 1;
        }
        
        static int Lua_EllipsoidSurface(lua_State *L)
        {
            static const double _p = 1.6075;
            
            const double a = lua_tonumber(L,1);
            const double b = lua_tonumber(L,2);
            const double c = lua_tonumber(L,3);
            
            const double ab = a*b;
            const double ac = a*c;
            const double bc = b*c;
            
            const double num = pow(ab,_p) + pow(ac,_p) + pow(bc,_p);
            const double S   = (4.0*numeric<double>::pi) * pow(num/3.0,1.0/_p);
            lua_pushnumber(L,S);
            return 1;
        }
        
        static int Lua_EllipsoidVolume(lua_State *L)
        {
            
            const double a = lua_tonumber(L,1);
            const double b = lua_tonumber(L,2);
            const double c = lua_tonumber(L,3);
            
            const double V   = (4.0*numeric<double>::pi*a*b*c)/3.0;
            lua_pushnumber(L,V);
            return 1;
        }


        
        
        void __lua:: register_functions(lua_State *L)
        {
            assert(L);
            lua_settop(L,0);
            lua_pushcfunction(L, Lua_GHK_Psi);
            lua_setglobal(L, "Psi");
            
            lua_pushcfunction(L, Lua_EllipsoidSurface);
            lua_setglobal(L, "EllipsoidSurface");
            
            lua_pushcfunction(L, Lua_EllipsoidVolume);
            lua_setglobal(L, "EllipsoidVolume");

            lua_pushnumber(L, Y_R);
            lua_setglobal(L, "R");

            lua_pushnumber(L, Y_FARADAY);
            lua_setglobal(L, "F");

            
        }
        
    }
}

