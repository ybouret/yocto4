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
            std::cerr << "Parsing " << n << " constraints" << std::endl;
            
            for(size_t i=1;i<=n;++i)
            {
                lua_rawgeti(L,-1, i);
                __parse_constraint(L,loader,txt,lib,i);
                
                lua_pop(L,1);
            }
            
            
            
        }
        
        
    }
    
}

