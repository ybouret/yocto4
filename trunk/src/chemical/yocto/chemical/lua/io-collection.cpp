#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        static inline
        void __parse_one_species( lua_State   *L,
                                 collection   &lib,
                                 const char   *table_name,
                                 const size_t  table_indx,
                                 species_ctor *ctor)
        {
            assert( lua_istable(L, -1));
            const size_t n  = lua_rawlen(L,-1);
            
            //------------------------------------------------------------------
            // get the name
            //------------------------------------------------------------------
            if(n<1)
                throw exception("%s[%u] has no species name", table_name, unsigned(table_indx));
            lua_rawgeti(L, -1, 1);
            if(!lua_isstring(L, -1))
                throw exception("%s[%u][1] is not a string", table_name, unsigned(table_indx));
            const string name = lua_tostring(L, -1);
            lua_pop(L, 1);
            
            //------------------------------------------------------------------
            // get the charge
            //------------------------------------------------------------------
            if(n<2)
                throw exception("%s[%u] has no species charge", table_name, unsigned(table_indx));
            lua_rawgeti(L, -1, 2);
            if(!lua_isnumber(L, -1))
                throw exception("%s[%u][1] is not a numer", table_name, unsigned(table_indx));
            const int z = int(lua_tonumber(L, -1));
            lua_pop(L, 1);
            species &sp = lib.add(name, z);
            
            if(ctor)
            {
                if(n>2)
                {
                    lua_rawgeti(L, -1, 3);
                    (*ctor)(L,sp);
                    lua_pop(L,1);
                }
                else
                {
                    lua_pushnil(L);
                    (*ctor)(L,sp);
                    lua_pop(L,1);
                }
            }
        }
        
        void _lua::load( lua_State *L,  collection &lib, const string &name, species_ctor *ctor)
        {
            assert(L);
            lua_settop(L,0);
            const char *table_name = name.c_str();
            lua_getglobal(L, table_name );
            if( ! lua_istable(L, -1) )
                throw exception("chemical collection '%s' is not a LUA_TABLE", table_name);
            
            const size_t n = lua_rawlen(L,-1);
            
            for(size_t i=1;i<=n;++i)
            {
                //--------------------------------------------------------------
                // get the species table
                //--------------------------------------------------------------
                lua_rawgeti(L, -1, i);
                
                //--------------------------------------------------------------
                // parse it
                //--------------------------------------------------------------
                if( ! lua_istable(L, -1) )
                    throw exception("'%s': item #%u is not a LUA_TABLE", table_name, unsigned(i) );
                
                __parse_one_species(L, lib, table_name, i, ctor);
                
                //--------------------------------------------------------------
                // remove the species table
                //--------------------------------------------------------------
                lua_pop(L,1);
            }
            
            lib.update_indices();
            
        }
        
        void _lua::load( lua_State *L,  collection &lib, const char *name, species_ctor *ctor)
        {
            const string NAME(name);
            _lua::load(L,lib,NAME,ctor);
        }
        
    }
}
