#include "yocto/chemical/lua/io.hpp"
#include "yocto/exception.hpp"


namespace yocto
{
    namespace chemical
    
    {
        
        static inline
        void __parse_constraint(lua_State     *L,
                                boot          &loader,
                                const char    *id,
                                const library &lib,
                                const unsigned i
                                )
        {
            if(!lua_istable(L, -1))
            {
                throw exception("%s[%u] is not a LUA_TABLE",id,i);
            }
            
            const unsigned n = lua_rawlen(L,-1);
            //__________________________________________________________________
            //
            // read the constraint level
            //__________________________________________________________________
            lua_rawgeti(L,-1, 1);
            if(!lua_isnumber(L,-1))
                throw exception("%s[%u][1] is not a LUA_NUMBER",id,i);
            const double level = lua_tonumber(L, -1);
            lua_pop(L, 1);
            
            constraint::pointer p( new constraint(level) );
            
            //__________________________________________________________________
            //
            // read the members
            //__________________________________________________________________
            for(unsigned j=2;j<=n;++j)
            {
                const unsigned k=j-1;
                lua_rawgeti(L,-1, j);
                if(!lua_istable(L, -1))
                    throw exception("%s[%u]: member #%u is not a LUA_TABLE",id,i,k);
                
                //______________________________________________________________
                //
                // read one member
                //______________________________________________________________
                {
                    lua_rawgeti(L, -1, 1);
                    if(!lua_isnumber(L,-1))
                    {
                        throw exception("%s[%u]: member #%u first entry is not a LUA_NUMBER",id,i,k);
                    }
                    const int weight = int(lua_tonumber(L,-1));
                    lua_pop(L, 1);
                    
                    lua_rawgeti(L, -1, 2);
                    if(!lua_isstring(L,-1))
                    {
                        throw exception("%s[%u]: member #%u second entry is not a LUA_STRING",id,i,k);
                    }
                    const string name = lua_tostring(L, -1);
                    lua_pop(L, 1);
                    p->add(lib[name], weight);
                }
                
                
                lua_pop(L,1);
            }
            
            if(p->count()<=0)
            {
                throw exception("%s[%u] is an empty constraint",id,i);
            }
            loader.push_back(p);
        }
        
        
        void __lua:: load(lua_State *L, boot &loader, const string &id, const library &lib)
        {
            assert(L);
            lua_settop(L,0);
            const char *txt = id.c_str();
            lua_getglobal(L, txt);
            if(!lua_istable(L, -1))
            {
                throw exception("%s is not a LUA_TABLE",txt);
            }
            const size_t n = lua_rawlen(L, -1);
            
            for(size_t i=1;i<=n;++i)
            {
                lua_rawgeti(L,-1, i);
                __parse_constraint(L,loader,txt,lib,i);
                
                lua_pop(L,1);
            }
        }
        
        
        void __lua:: load(lua_State *L, boot &loader, const char *id, const library &lib)
        {
            const string ID(id);
            load(L,loader,ID,lib);
        }

        
    }
    
}

