//! \file
/**
 Reaction And Diffusion, Basic Layout Example
 */
#ifndef YOCTO_AQUEOUS_RAMBLE_INCLUDED
#define YOCTO_AQUEOUS_RAMBLE_INCLUDED 1


#include "yocto/aqueous/lua.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/lua/lua-config.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
    
    namespace aqueous
    {
        
        namespace ramble
        {
            template <
            typename T,
            typename DATA_ARRAY,
            typename FLUX_ARRAY>
            struct of
            {
                
                class species_data
                {
                public:
                    explicit species_data( T diffusion_coefficient ) throw() :
                    D( diffusion_coefficient ),
                    U(0),
                    F(0),
                    I(0)
                    {
                    }
                    
                    virtual ~species_data() throw() {}
                    
                    const T      D;
                    DATA_ARRAY  *U; //!< concentration field
                    FLUX_ARRAY  *F; //!< flux field
                    DATA_ARRAY  *I; //!< increase field
                    
                    //! called after construction
                    /**
                     The table of EXTRA parameters is on the stack
                     */
                    virtual void on_load( const species &sp, lua_State *L )
                    {
                        if( !lua_istable(L,-1) )
                            throw exception("%s: no lua table", sp.name.c_str());
                        const size_t ni = lua_rawlen(L, -1); assert(ni>0);
                        std::cerr << "***\t\t" << sp.name << " has " << ni-1 << " extra info" << std::endl;
                    }
                    
                    //! function for species callback
                    /**
                     the EXTRA term of the species must be a table,
                     with the first item being D
                     */
                    static inline
                    void ctor( species &sp, lua_State *L )
                    {
                        //----------------------------------------------------------
                        // check table
                        //----------------------------------------------------------
                        const char *id = sp.name.c_str();
                        std::cerr << "***\t\tLoading Data for " << id << std::endl;
                        if( !lua_istable(L,-1) )
                            throw exception("%s: EXTRA term is not a table", id);
                        
                        //----------------------------------------------------------
                        // extract the diffusion coefficient
                        //----------------------------------------------------------
                        lua_rawgeti(L, -1, 1); //---- push the diffusion coefficient
                        if( ! lua_isnumber(L,-1) )
                            throw exception("%s: first item is not a number", id);
                        sp.make<species_data,T>( lua_tonumber(L, -1) );
                        lua_pop(L,1);          //----  pop the diffusion coefficient
                        species_data &self = sp.get<species_data>();
                        std::cerr << "***\t\tD_{" << sp.name <<  "} = " << self.D << std::endl;
                        
                        //----------------------------------------------------------
                        // virtual extra loading
                        //----------------------------------------------------------
                        self.on_load(sp,L);
                    }
                    
                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(species_data);
                };
                
                class library : public aqueous::library
                {
                public:
                    explicit library( const string &libname, lua_State *L ) :
                    aqueous::library( sizeof(species_data) )
                    {
                        load_species_from( libname, L);
                    }
                    
                    explicit library( const char *libname, lua_State *L ) :
                    aqueous::library( sizeof(species_data) )
                    {
                        const string id( libname );
                        load_species_from(id, L);
                    }
                    
                    virtual ~library() throw()
                    {
                    }
                    
                private:
                    YOCTO_DISABLE_COPY_AND_ASSIGN(library);
                    void load_species_from( const string &libname, lua_State *L )
                    {
                        _lua::species_ctor ctor( cfunctor2( species_data::ctor ) );
                        _lua::load(L, *this, libname, &ctor);
                    }
                };
                
            };
            
            class chemsys : public aqueous::chemsys
            {
            public:
                explicit chemsys( const library &l, const string &csname, lua_State *L, const string &ftol_name ) :
                aqueous::chemsys(l, Lua::Config::Get<lua_Number>(L,ftol_name) )
                {
                    _lua::load(L,*this, csname);
                    build();
                }
                
                virtual ~chemsys() throw() {}
                
            private:
                YOCTO_DISABLE_COPY_AND_ASSIGN(chemsys);
            };
            
        }
    }
    
}



#endif
