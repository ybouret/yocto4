#ifndef YOCTO_AQUEOUS_LUA_INCLUDED
#define YOCTO_AQUEOUS_LUA_INCLUDED 1

#include "yocto/aqueous/initializer.hpp"
#include "yocto/sequence/vector.hpp"
#include "yocto/associative/labelled.hpp"

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
            //! transform data on into of stack into a C++ data
            typedef functor<void,TL2(species&,lua_State*)> species_ctor;
            
            //! load a table of { "name", charge, EXTRA }
            /**
             \param L a lua state
             \param lib the library to fill
             \param the lua table name to load
             \param cb  an optional ctor
             if the callback is present, then the extra term is pushed onto the lua_State
             then the callback is called for the current species.
             */
            static void load( lua_State *L, library     &lib, const string &libname, species_ctor *cb = NULL);
            static void load( lua_State *L, library     &lib, const char   *libname, species_ctor *cb = NULL);
            
            //! load a table of chemical equilibria
            static void load( lua_State *L, chemsys     &cs,  const string &csname  );
            static void load( lua_State *L, chemsys     &cs,  const char   *csname  );

            
            //! load a table of chemical initializer
            static void load( lua_State *L, initializer &ini, const string &ininame );
            static void load( lua_State *L, initializer &ini, const char   *ininame );

            //! effector
            class effector : public object, public counted
            {
            public:
                const string   name;    //!< will be the function name
                vector<string> input;   //!< input  arguments names
                vector<string> output;  //!< output arguments names
                double         factor;  //!< scaling factor, default=1
                
                const string & key() const throw();
                typedef intr_ptr<string,effector>      ptr;
                typedef labelled<set,effector::ptr>    db;
                explicit effector( const string &id );
                virtual ~effector() throw();
                
                //! append results to dSdt
                void call( lua_State *L, solution &dSdt, double t, double zeta, const solution &S ) const;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(effector);
            };
            
            class effectors : public effector::db
            {
            public:
                explicit effectors() throw();
                virtual ~effectors() throw();
                
                effector & operator[]( const string &name );
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(effectors);
            };
            
            //! load a table of effectors
            static void load( lua_State *L, effectors &db, const string &effname );
            static void load( lua_State *L, effectors &db, const char   *effname );
            
        };
        
    }
    
}


#endif
