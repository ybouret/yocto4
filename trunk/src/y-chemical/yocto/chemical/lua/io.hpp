#ifndef YOCTO_CHEMICAL_LUA_IO_INCLUDED
#define YOCTO_CHEMICAL_LUA_IO_INCLUDED 1

#include "yocto/chemical/initializer.hpp"

extern "C"
{
    struct lua_State;
}

namespace yocto
{
    namespace chemical
    {
        
        typedef functor<void, TL2(lua_State*,species&)> species_ctor;
        
        struct _lua
        {
            
           
            
            static void load( lua_State *L, collection &lib, const string &name, species_ctor * =0 );
            static void load( lua_State *L, collection &lib, const char   *name, species_ctor * =0 );

         
            class equilibrium;
            static void load( lua_State *L, const collection &lib, equilibria &cs, const string &name );
            static void load( lua_State *L, const collection &lib, equilibria &cs, const char   *name );

            static void load( lua_State *L, initializer &ini, const string &name );
            
            
        };
        
    }
    
}

#endif
