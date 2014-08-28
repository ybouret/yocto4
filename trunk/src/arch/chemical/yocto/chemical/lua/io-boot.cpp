#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        static inline
        void __parse_one_constraint(lua_State        *L,
                                    boot::loader     &ini,
                                    const collection &lib,
                                    const char       *table_name,
                                    const unsigned    table_indx)
        {
            //std::cerr << "Parsing " << table_name << "[" << table_indx << "]" << std::endl;
            if( !lua_istable(L, -1))
                throw exception("%s[%u] is not a LUA_TABLE", table_name, table_indx);
            
            const size_t n = lua_rawlen(L, -1);
            
            //------------------------------------------------------------------
            // get the constraint value
            //------------------------------------------------------------------
            if(n<1)
                throw exception("%s[%u] missing constraint value",table_name, table_indx);
            lua_rawgeti(L,-1,1); // get the constraint value -------------------
            if(!lua_isnumber(L, -1))
                throw exception("%s[%u] invalid constant", table_name, table_indx);
            const double value = lua_tonumber(L, -1);
            lua_pop(L, 1);       // pop the constraint value--------------------
            
            
            boot::constraint &c = ini.add(value);
            
            
            //------------------------------------------------------------------
            // parse the components
            //------------------------------------------------------------------
            for(size_t i=2; i<=n;++i)
            {
                const unsigned j = i-1;
                assert( lua_istable(L, -1 ) );
                lua_rawgeti(L, -1, i); // get the component---------------------
                if( !lua_istable(L,-1) )
                    throw exception("%s[%u] invalid type for component #%u", table_name, table_indx, j);
                
                if( 2 != lua_rawlen(L,-1) )
                    throw exception("%s[%u] invalid format for component #%u", table_name, table_indx, j);
                
                //-- get the weight
                lua_rawgeti(L, -1, 1);
                if(!lua_isnumber(L, -1 ))
                    throw exception("%s[%u] component #%u: invalid weight", table_name, table_indx, j);
                const double w = lua_tonumber(L, -1);
                lua_pop(L,1);
                
                //-- get the name
                lua_rawgeti(L, -1, 2);
                if(!lua_isstring(L, -1))
                    throw exception("%s[%u] component #%u: invalid name", table_name, table_indx, j);
                const string name = lua_tostring(L, -1);
                lua_pop(L,1);
                
                const species::ptr *pp = lib.search(name);
                if(!pp)
                    throw exception("%s[%u]: component %u: unknown species '%s'", table_name, table_indx, j, name.c_str());
                
                const int W = int(w);
                if(W==0)
                    throw exception("%s[%u]: component %u: weight for '%s' becomes ZERO", table_name, table_indx, j, name.c_str());
                (void)c.weight( *pp, W);
                
                lua_pop(L,1);         // pop the component---------------------
            }
            
        }
        
        
        void _lua:: load( lua_State *L, boot::loader &ini, const string &name, const collection &lib)
        {
            assert(L);
            lua_settop(L,0);
            const char *table_name = name.c_str();
            lua_getglobal(L, table_name);
            if( !lua_istable(L, -1) )
                throw exception("initializer '%s' is not a LUA_TABLE", table_name );
            
            const size_t n = lua_rawlen(L,-1);
            std::cerr << "parsing " << n << " init" << std::endl;
            for(size_t i=1; i<=n; ++i)
            {
                //--------------------------------------------------------------
                // push constraint
                //--------------------------------------------------------------
                lua_rawgeti(L, -1, i);
                
                __parse_one_constraint(L, ini, lib, table_name, i);
                
                //--------------------------------------------------------------
                // pop constraint
                //--------------------------------------------------------------
                lua_pop(L,1);
            }
        }
        
        void _lua:: load( lua_State *L, boot::loader &ini, const char *name, const collection &lib )
        {
            const string NAME(name);
            return _lua::load(L,ini,NAME,lib);
        }
        
    }
    
}

