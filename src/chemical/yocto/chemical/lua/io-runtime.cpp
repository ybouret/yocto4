#include "yocto/chemical/lua/io.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        void _lua:: push_solution( lua_State *L, const solution &S )
        {
            assert(L);
            const size_t nrec = S.components;
            lua_createtable(L, 0, nrec);
            for( solution::const_iterator i=S.begin();i!=S.end();++i)
            {
                const component &cc    = *i;
                const string    &key   = cc.key();
                const double     value = cc.concentration;
                
                lua_pushlstring(L, key.c_str(), key.size());
                lua_pushnumber(L,value);
                lua_rawset(L, -3);
            }
        }
        
        void _lua:: push_solution( lua_State *L, const collection &lib, const array<double> &C)
        {
            assert(L);
            assert(C.size()>=lib.size());
            const size_t nrec = lib.size();
            lua_createtable(L, 0, nrec);
            
            collection::const_iterator i = lib.begin();
            for(size_t j=1;j<=nrec;++j,++i)
            {
                const species &sp = **i;
                lua_pushlstring(L, sp.name.c_str(), sp.name.size());
                lua_pushnumber(L, C[j]);
                lua_rawset(L,-3);
            }
        }

        
    }
}

