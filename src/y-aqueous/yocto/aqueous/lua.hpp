#ifndef YOCTO_AQUEOUS_LUA_INCLUDED
#define YOCTO_AQUEOUS_LUA_INCLUDED 1

#include "yocto/aqueous/initializer.hpp"
#include "yocto/sequence/vector.hpp"

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
            static void load( lua_State *L, library     &lib, const string &libname, species_ctor *cb = NULL);

            static void load( lua_State *L, chemsys     &cs,  const string &csname  );
            static void load( lua_State *L, initializer &ini, const string &ininame );
            
            //! effector
            class effector : public object, public counted
            {
            public:
                const string   name;    //!< will be the function name
                vector<string> input;   //!< input arguments
                vector<string> output;  //!< output arguments
                
                const string & key() const throw();
                typedef intrusive_ptr<string,effector> ptr;
                typedef set<string,effector::ptr>      db;
                explicit effector( const string &id );
                virtual ~effector() throw();
                
                //! append results to dSdt
                void call( lua_State *L, solution &dSdt, double t, const solution &S ) const;
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(effector);
            };
            
            static void load( lua_State *L, effector::db &effectors, const string &effname );
            
        };
        
    }
    
}


#endif
