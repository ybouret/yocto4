#ifndef YOCTO_CHEMICAL_LUA_IO_INCLUDED
#define YOCTO_CHEMICAL_LUA_IO_INCLUDED 1

#include "yocto/chemical/library.hpp"
#include "lua.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    namespace chemical
    {
        
        struct __lua
        {
            typedef functor<void,TL2(lua_State *,species&)> species_callback;
            
            //! append to library some species
            /**
             \param L a vali lua State
             \param lib a library, should be unlocked
             \param id the name of the lua table
             example:
             species = { { "H+", 1, {extra_data} }, { "HO-", -1, {extra_data} } }
             */
            static void load(lua_State *L, library &lib, const string &id, species_callback *cb = 0 );
            
            //! wrapper
            static void load(lua_State *L, library &lib, const char   *id, species_callback *cb = 0 );
        };
        
    }
}

#endif

