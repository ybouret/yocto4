#include "yocto/chemical/lua/io.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        
        static inline
        void __parse_species(lua_State     *L,
                             library       &lib,
                             const char    *id,
                             const unsigned i,
                             __lua::species_callback *cb)
        {
            
            if( !lua_istable(L, -1) )
            {
                throw exception("%s[%u] is not a LUA_TABLE", id, i);
            }
            
            // get the name
            lua_rawgeti(L, -1, 1);
            if( !lua_isstring(L, -1) )
            {
                throw exception("%s[%u] name is no a string", id, i);
            }
            const string name = lua_tostring(L, -1);
            lua_pop(L, 1);

            // get the charge
            lua_rawgeti(L, -1, 2);
            if( !lua_isnumber(L, -1) )
            {
                throw exception("%s['%s'] charge is not a number ", id, name.c_str());
            }
            species &sp = lib.add(name,int(lua_tonumber(L,-1)));
            lua_pop(L,1);
            
            
            // get extra data
            if(cb)
            {
                lua_rawgeti(L, -1, 3);
                (*cb)(L,sp);
                lua_pop(L,1);
            }
        }
        
        void __lua::load(lua_State *L, library &lib, const string &id, species_callback *cb )
        {
            lua_settop(L,0);
            lua_getglobal(L,id.c_str());
            if( !lua_istable(L, -1) )
            {
                throw exception("_lua::load.library: %s is not a LUA_TABLE", id.c_str());
            }
            const size_t n = lua_rawlen(L, -1);
            
            for(size_t i=1;i<=n;++i)
            {
                lua_rawgeti(L,-1,i);
                __parse_species(L,lib,id.c_str(),i,cb);
                lua_pop(L,1);
            }
            
            
        }
        
        
        void __lua::load(lua_State *L, library &lib, const char *id, species_callback *cb)
        {
            const string ID(id);
            __lua::load(L,lib,ID,cb);
        }
    }

}
