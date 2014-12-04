#include "yocto/chemical/lua/io.hpp"

namespace yocto
{
    namespace chemical
    {
        void __lua:: load(lua_State *L, matrix_t solutions, const string &id)
        {
            assert(L);
            const char *name = id.c_str();
            lua_settop(L,0);
            lua_getglobal(L,name);
        }
        
    }
    
}
