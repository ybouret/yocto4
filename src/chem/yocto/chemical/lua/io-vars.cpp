#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        
        void __lua:: push(lua_State *L,
                          const array<double> &A,
                          const variables     &params)
        {
            assert(L);
            variables::iterator  p = params.begin();
            const size_t         n = params.extra;
            lua_createtable(L, 0, n);
            for(size_t i=1;i<=n;++i,++p)
            {
                const variable &v     = **p;
                const string   &key   = v.name; assert(v.indx>0);assert(v.indx<=A.size());
                const size_t    num   = v.indx; assert(num>0);assert(num<=A.size());
                const double    value = A[ num ];
                lua_pushlstring(L, key.c_str(), key.size());
                lua_pushnumber(L,value);
                lua_rawset(L, -3);
            }
        }

        void __lua:: load( lua_State *L, array<double> &A, const variables &params)
        {
            assert(L);
            for(variables::iterator i=params.begin();i!=params.end();++i)
            {
                const variable &v  = **i;
                const string   &ld = v.load;
                assert(v.indx>0);assert(v.indx<=A.size());
                lua_settop(L, 0);
                lua_getglobal(L,ld.c_str());
                if( !lua_isnumber(L, -1) )
                {
                    throw exception("loading '%s': '%s' is not a number", v.name.c_str(), v.load.c_str());
                }
                lua_settop(L,0);
            }
        }

    }
    
}

