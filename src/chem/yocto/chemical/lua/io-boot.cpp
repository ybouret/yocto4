#include "yocto/chemical/lua/io.hpp"
#include "yocto/exception.hpp"


namespace yocto
{
    namespace chemical

    {

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
            
        }


    }

}

