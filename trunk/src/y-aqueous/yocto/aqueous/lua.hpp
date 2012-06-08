#ifndef YOCTO_AQUEOUS_LUA_INCLUDED
#define YOCTO_AQUEOUS_LUA_INCLUDED 1

#include "yocto/aqueous/initializer.hpp"

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
            typedef functor<void,TL2(species&,lua_State*)> species_ctor;
            
            static void load( lua_State *L, library     &lib, const string &libname, species_ctor *cb = NULL);
            static void load( lua_State *L, chemsys     &cs,  const string &csname  );
            static void load( lua_State *L, initializer &ini, const string &ininame );
            
        };
        
    }
    
}


#endif
