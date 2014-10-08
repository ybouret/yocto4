#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        static inline
        void __parse_condition( const collection &lib, boot &loader, lua_State *L, const char *id, const unsigned idx)
        {
            if( !lua_istable(L, -1))
                throw exception("'%s' #%u: not a LUA_TABLE",id,idx);
            
            const unsigned n= lua_rawlen(L, -1);
            if(n<2)
                throw exception("'%s' #%u: need at least { constant, {coef, \"species\"}} ",id,idx);
            
            //__________________________________________________________________
            //
            // get the constant part
            //__________________________________________________________________
            lua_rawgeti(L, -1, 1);
            if(!lua_isnumber(L, -1))
                throw exception("'%s' #%u: first item is not a LUA_NUMBER",id,idx);
            boot::constraint &CC = loader.create( lua_tonumber(L, -1) );
            lua_pop(L,1);
            
            
            //__________________________________________________________________
            //
            // get the items
            //__________________________________________________________________
            for(unsigned i=2;i<=n;++i)
            {
                //______________________________________________________________
                //
                // get the sub table: {index,species}
                //______________________________________________________________
                lua_rawgeti(L, -1, i);
                
                if( !lua_istable(L, -1))
                    throw exception("'%s' #%u: item[%u] is not a LUA_TABLE",id,idx,i);
                
                //__________________________________________________________
                //
                // get the first item: weight
                //__________________________________________________________
                lua_rawgeti(L, -1, 1);
                if(!lua_isnumber(L, -1))
                    throw exception("'%s' #%u: item[%u][1] is not a LUA_NUMBER",id,idx,i);
                const int weight = int(lua_tonumber(L, -1));
                //__________________________________________________________
                //
                // remove the first item
                //__________________________________________________________
                lua_pop(L,1);
                
                
                //__________________________________________________________
                //
                // get the second item: weight
                //__________________________________________________________
                lua_rawgeti(L, -1, 2);
                if(!lua_isstring(L, -1))
                    throw exception("'%s' #%u: item[%u][1] is not a LUA_STRING",id,idx,i);
                const string name = lua_tostring(L,-1);
                //__________________________________________________________
                //
                // remove the second item
                //__________________________________________________________
                lua_pop(L,1);

                
                
                //______________________________________________________________
                //
                // remove the sub-table
                //______________________________________________________________
                lua_pop(L,1);
                
                CC.weight(lib[name], weight);
            }
            
            
            
        }
        
        
        void _lua:: load( lua_State *L, const collection &lib, boot &loader, const string &name)
        {
            assert(L!=NULL);
            
            lua_settop(L, 0);
            lua_getglobal(L,name.c_str());
            
            const char *id = name.c_str();
            if(!lua_istable(L, -1))
                throw exception("boot loader: '%s' is not a LUA_TABLE",id);
            
            const unsigned n = lua_rawlen(L, -1);
            std::cerr << "Parsing " << n << " loading conditions" << std::endl;
            for(unsigned i=1;i<=n;++i)
            {
                //______________________________________________________________
                //
                // push the sub-table
                //______________________________________________________________
                lua_rawgeti(L, -1, i);
                
                __parse_condition(lib, loader,L, id, i);
                
                //______________________________________________________________
                //
                // pop the sub-table
                //______________________________________________________________
                lua_pop(L,1);
            }
        }
        
        
    }
    
}

