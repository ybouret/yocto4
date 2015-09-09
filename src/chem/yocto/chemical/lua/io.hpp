#ifndef YOCTO_CHEMICAL_LUA_IO_INCLUDED
#define YOCTO_CHEMICAL_LUA_IO_INCLUDED 1

#include "yocto/chemical/equilibria.hpp"
#include "yocto/chemical/boot.hpp"
#include "yocto/chemical/effectors.hpp"

#include "yocto/lua/lua.hpp"
#include "yocto/functor.hpp"

namespace yocto
{
    namespace chemical
    {
        
        struct __lua
        {
            
            //! register Psi, EllipsoidSurface and EllipsoidVolume, R and F
            static void register_functions(lua_State *L);
            
            //__________________________________________________________________
            //
            // library API
            //__________________________________________________________________
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
            
            //__________________________________________________________________
            //
            // equilibria API
            //__________________________________________________________________
            //! append to equilibria
            static void load(lua_State *L, equilibria &eqs,  const string &id , const library &lib);
            
            //! append to equilibria, wrapper
            static void load(lua_State *L, equilibria &eqs,  const char *id , const library &lib);
            
            //__________________________________________________________________
            //
            // boot API
            //__________________________________________________________________
            //! append to boot
            static void load(lua_State *L, boot &loader, const string &id, const library &lib);
            
            
            //! append to boot, wrapper
            static void load(lua_State *L, boot &loader, const char *id, const library &lib);
            
            
            //__________________________________________________________________
            //
            // solution API
            //__________________________________________________________________
            //! push array as a solution
            static void push( lua_State *L, const array<double> &S, const library &lib);
            
            
            //! read solution on stack
            static void load( lua_State *L, array<double> &S, const library &lib);
            
            //__________________________________________________________________
            //
            // parameters API
            //__________________________________________________________________
            //! push a table of parameters from the variables
            static void push( lua_State *L, const array<double> &A, const variables &params);

            //! load values from params loading name
            static void load( lua_State *L, array<double> &A, const variables &params);

            //__________________________________________________________________
            //
            // effectors API
            //__________________________________________________________________
            static void load( lua_State *L, effectors &edb, const string &id);
    
            static void load( lua_State *L, effectors &edb, const char   *id);

            
            //__________________________________________________________________
            //
            // mix API
            //__________________________________________________________________
            static void load(lua_State     *L,
                             matrix_t      &solutions,
                             const string  &id,
                             const library &lib,
                             equilibria    &eqs,
                             const double   t);
            
            //__________________________________________________________________
            //
            // Classes to help
            //__________________________________________________________________

            //! Load the library with the given ID
            class Library : public library
            {
            public:
                explicit Library( lua_State *L, const char *id );
                virtual ~Library() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Library);
            };


            //! load the equilibria from the table with the given ID
            class Equilibria : public equilibria
            {
            public:
                // load and compile
                explicit Equilibria(lua_State *L, const char *id, const library &lib);
                virtual ~Equilibria() throw();
                
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Equilibria);
            };
            
            //! load the effectors from the table with the given ID
            class Effectors : public effectors
            {
            public:
                explicit Effectors(lua_State *L, const char *id);
                virtual ~Effectors() throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(Effectors);
            };
        };
        
    
        
    }
}

#endif

