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
            
            class equilibrium : public chemical::equilibrium
            {
            public:
                explicit equilibrium( const string &name, lua_State *L, const string &fn );
                virtual ~equilibrium() throw();
                virtual double getK( double t ) const throw();
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(equilibrium);
            };
            
            static void load( lua_State *L, collection &lib, const string &name, species_ctor * =0 );
            static void load( lua_State *L, collection &lib, const char   *name, species_ctor * =0 );

        };
        
    }
    
}

#endif
