#include "yocto/chemical/lua/io.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    namespace chemical
    {
        void __lua:: load(lua_State     *L,
                          matrix_t      &solutions,
                          const string  &id,
                          const library &lib,
                          equilibria    &eqs,
                          const double   t)
        {
            // get the array of names
            assert(L);
            const char *name = id.c_str();
            lua_settop(L,0);
            lua_getglobal(L,name);

            if(!lua_istable(L, -1))
            {
                throw exception("'%s' is not a LUA_TABLE of initial conditions",name);
            }

            // parse all strings to use other function
            const size_t   ns = lua_rawlen(L, -1);
            solutions.make(ns,eqs.M);

            vector<string> vs(ns,as_capacity);
            for(size_t i=1;i<=ns;++i)
            {
                lua_rawgeti(L, -1, i);
                if( !lua_isstring(L, -1))
                {
                    throw exception("%s[%u] is not a string", name, unsigned(i));
                }
                const string tmp = lua_tostring(L, -1);
                vs.push_back(tmp);
                lua_pop(L,1);
            }

            // now loading
            for(size_t i=1;i<=ns;++i)
            {
                const string ini_name = vs[i];
                boot         loader;
                load(L,loader,ini_name,lib);
                eqs.create(solutions[i], loader, t);
            }
        }
        
    }
    
}
