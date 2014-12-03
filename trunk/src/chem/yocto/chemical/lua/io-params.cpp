#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        void __lua:: push(lua_State *L,
                          const array<double> &variables,
                          const parameters    &parameters)
        {
            assert(L);
            parameters::iterator p = parameters.begin();
            const size_t         n = parameters.count;
            lua_createtable(L, 0, n);
            for(size_t i=1;i<=n;++i,++p)
            {
                const string &key   = p->key;
                const size_t  num   = *p; assert(num>0);assert(num<=variables.size());
                const double  value = variables[ num ];
                lua_pushlstring(L, key.c_str(), key.size());
                lua_pushnumber(L,value);
                lua_rawset(L, -3);
            }
        }
        
    }
    
}

