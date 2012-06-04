#ifndef YOCTO_AQUEOUS_LUA_INCLUDED
#define YOCTO_AQUEOUS_LUA_INCLUDED 1

#include "yocto/aqueous/chemsys.hpp"

extern "C"
{
    struct lua_State;
}


namespace yocto 
{
    
    namespace aqueous
    {
        
        
        struct _lua
        {
            
            static void load( lua_State *L, library &lib, const string &libname);
            static void load( lua_State *L, chemsys &cs,  const string &csname );
        };
        
    }
    
}


#endif
