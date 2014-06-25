#ifndef YOCTO_CHEM_LUA_INCLUDED
#define YOCTO_CHEM_LUA_INCLUDED 1

#include "yocto/chemical/collection.hpp"
#include "yocto/functor.hpp"

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
            
            //__________________________________________________________________
            //
            // collection API
            //__________________________________________________________________
            
            //! append species to a collection
            /**
             \param L a  valid lua_State
             \param lib  collection to grow
             \param name the name of the lua table
             \param cb   a constructor for extra data
             
             when cb is not NULL, the extra data are pushed onto the state and
             cb is called: a nil value is pushed if no extra data are present.
             
             Indices are updated.
             */
            static void load( lua_State *L, collection &lib, const string &name, species_ctor *cb =0 );
            
            //! wrapper
            static void load( lua_State *L, collection &lib, const char   *name, species_ctor *cb =0 );
            
        };
        
        
    }
    
}

#endif
