#ifndef YOCTO_CHEMICAL_LUA_IO_INCLUDED
#define YOCTO_CHEMICAL_LUA_IO_INCLUDED 1

#include "yocto/chemical/boot.hpp"
#include "yocto/chemical/effector.hpp"

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

            //__________________________________________________________________
            //
            // equilibria API
            //__________________________________________________________________

            //! forward lua equilibrium
            class equilibrium;
            
            //! append equilibria to a chemical system
            /**
             \param L a valid lua_State
             \param lib collection of species
             \param cs  the equilibria
             \param name the name of the lua table
             */
            static void load( lua_State *L, const collection &lib, equilibria &cs, const string &name );
            
            //! wrapper
            static void load( lua_State *L, const collection &lib, equilibria &cs, const char   *name );

            
            //__________________________________________________________________
            //
            // boot API
            //__________________________________________________________________
            
            //! append constraints to a boot::loader
            /**
             \param L a valid lua_State
             \param ini a boot loader
             \param name the name of the lua table
             \param lib the collection to search for species
             */
            static void load( lua_State *L, boot::loader &ini, const string &name, const collection &lib);
            
            //! wrapper
            static void load( lua_State *L, boot::loader &ini, const char *name, const collection &lib);

            //__________________________________________________________________
            //
            // effector API
            //__________________________________________________________________

            //! forward lua effector
            class effector;
            
            //! append effectors
            /**
             \param L a valid lua_State
             \param eff the name of the effectors database
             \param name the name of the lua table
             */
            static void load( lua_State *L, effectors &eff, const string &name);
          
            //! wrapper
            static void load( lua_State *L, effectors &eff, const char *name);
            
            
            //__________________________________________________________________
            //
            // Helpers
            //__________________________________________________________________
            
            //! push a solution on a a lua stack
            static void push_solution( lua_State *L, const solution &S );
            
            //! push a solution on a lua stack, assume C is built from lib
            static void push_solution( lua_State *L, const collection &lib, const array<double> &C);
        };
        
    }
    
}

#endif
