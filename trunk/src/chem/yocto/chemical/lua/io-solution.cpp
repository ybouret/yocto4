#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        void _lua:: push(lua_State *L, const solution &S)
        {
            assert(L);
            const size_t M = S.size();
            lua_createtable(L, 0, M);
            collection::const_iterator j = S.lib.begin();
            for(size_t i=1;i<=M;++i,++j)
            {
                const string    &key   = (*j)->name;
                const double     value = S[i];
                
                lua_pushlstring(L, key.c_str(), key.size());
                lua_pushnumber(L,value);
                lua_rawset(L, -3);
            }
        }
        
        
        void _lua:: load(lua_State *L, solution &S )
        {
            static const char fn[] = "_lua::load(solution): ";
            
            if(!lua_istable(L,-1))
                throw exception("%sno  LUA_TABLE but '%s'", fn, lua_typename(L, lua_type(L, -1)));
            
            //--------------------------------------------------------------
            // parse the result
            //--------------------------------------------------------------
            const int ans = lua_gettop(L); // the result index
            lua_pushnil(L);  /* first key */
            while( lua_next(L,ans) != 0) {
                /* uses 'key' (at index -2) and 'value' (at index -1) */
                //printf("%s - %s\n",lua_typename(L, lua_type(L, -2)),lua_typename(L, lua_type(L, -1)));
                
                if( !lua_isstring(L,-2) )
                    throw exception("%skey is not 'string' but '%s'", fn, lua_typename(L, lua_type(L, -2)));
                
                if( !lua_isnumber(L,-1) )
                    throw exception("%svalue is not 'number' but '%s'", fn, lua_typename(L, lua_type(L, -1)));
                
                const string which  = lua_tostring(L,-2);
                const double value  = lua_tonumber(L,-1);
                S(which) = value;
                
                /* removes 'value'; keeps 'key' for next iteration */
                lua_pop(L, 1);
            }
            // remove the key helper
            lua_pop(L,1);
            
        }
        
    }
    
}