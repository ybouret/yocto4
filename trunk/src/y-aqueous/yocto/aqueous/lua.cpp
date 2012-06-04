#include "yocto/aqueous/lua.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/lua/lua-maths.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
    namespace aqueous
    {
        
        void _lua:: load( lua_State *L, library &lib, const string &libname)
        {
            //==================================================================
            // load the library table
            //==================================================================
            const char *lib_name = libname.c_str();
            lua_settop(L,0);
            lua_getglobal(L,lib_name);
            if( ! lua_istable(L,-1) )
                throw exception("%s is not a table", lib_name);
            
            //==================================================================
            // load the species
            //==================================================================
            const unsigned ns = lua_objlen(L, -1);
            std::cerr << "*** Parsing " << ns << " species" << std::endl;
            for( unsigned i=1; i <= ns; ++i )
            {
                //--------------------------------------------------------------
                // get the species data on the stack
                //--------------------------------------------------------------
                lua_rawgeti(L,-1,i);
                
                
                if( ! lua_istable(L, -1) )
                    throw exception("species #%u: not a table");
                
                //-- get the number of fields
                const unsigned nf = lua_objlen(L, -1);
                if( nf < 2 )
                    throw exception("species #%u: not enough fields");
                
                //-- get the id
                lua_rawgeti(L, -1, 1);
                if( !lua_isstring(L, -1) )
                    throw exception("species #%u: first field is not a string");
                size_t       len = 0;
                const char  *ptr = lua_tolstring(L, -1, &len);
                const string id(ptr,len);
                const char  *ch = id.c_str();
                lua_pop(L,1);
                
                //-- get the algebraic charge
                lua_rawgeti(L, -1, 2);
                if( !lua_isnumber(L, -1) )
                    throw exception("%s: second field is not a number",ch);
                const int z = int( lua_tonumber(L, -1) );
                std::cerr << "***\t<" << id << ">, z=" << z << std::endl;
                lua_pop(L,1);
                
                //-- create the species
                species &sp = lib.add(id, z);
                
                // TODO: further elements
                (void)sp;
                
                //--------------------------------------------------------------
                // remove the species data
                //--------------------------------------------------------------
                lua_pop(L,1);
            }
        }
        
        void _lua:: load(lua_State *L, yocto::aqueous::chemsys &cs, const string &csname)
        {
            //==================================================================
            // load the chemsys table
            //==================================================================
            const char *table_name = csname.c_str();
            lua_settop(L,0);
            lua_getglobal(L,table_name);
            if( ! lua_istable(L,-1) )
                throw exception("%s is not a table", table_name);
            
            //==================================================================
            // load the equilibria
            //==================================================================
            const unsigned neq = lua_objlen(L, -1);
            std::cerr << "*** Parsing " << neq << " equilibria" << std::endl;
            for( unsigned i=1; i <= neq; ++i )
            {
                //--------------------------------------------------------------
                // get the equilibrum data
                //--------------------------------------------------------------
                lua_rawgeti(L,-1,i);
                
                if( !lua_istable(L, -1) )
                    throw exception( "equilibrium #%u: not a table", i);
                
                const size_t nf = lua_objlen(L, -1);
                if( nf < 3 )
                    throw exception("equilibrium #%u: not enough fields",i);
                
                //-- get the name
                lua_rawgeti(L,-1,1);
                if( !lua_isstring(L, -1) )
                    throw exception("equilibrium #%u: first field is not a string", i);
                size_t       len = 0;
                const char  *ptr = lua_tolstring(L, -1, &len);
                const string id(ptr,len);
                const char  *ch = id.c_str();
                lua_pop(L,1);
                std::cerr << "***\t +[" << id << "]" << std::endl;
                
                //-- get the constant: function or constant
                equilibrium *eq = NULL;
                lua_rawgeti(L, -1,2);
                if( lua_isnumber(L,-1) )
                {
                    // create with a real constant
                    const double K = lua_tonumber(L, -1);
                    eq = & cs.create(id, K);
                    std::cerr << "***\t\t constant = " << K << std::endl;
                }
                else 
                {
                    // create with a function name
                    if( lua_isstring(L, -1) )
                    {
                        ptr = lua_tolstring(L, -1, &len);
                        const string fn(ptr,len);
                        std::cerr << "***\t\t using function '" << fn << "'" << std::endl;
                        Lua::Function<double>       kfn(L,fn); // functionoid
                        const equilibrium::constant K( kfn );  // functor
                        eq = & cs.create(id, K);
                    }
                    else 
                        throw exception("[%s]: invalid constant type", ch);
                }
                lua_pop(L,1);
                
                //-- parse the actors
                for( unsigned j=3,k=1; j <= nf; ++j, ++k )
                {
                    // get the actor table
                    lua_rawgeti(L,-1,j);
                    {
                        if( !lua_istable(L, -1) )
                            throw exception("[%s]: invalid actor #%u", ch, k );
                        if( lua_objlen(L, -1) != 2 )
                            throw exception("[%s]: invalid actor #%u format", ch, k );
                        
                        // get the actor name
                        lua_rawgeti(L, -1, 2);
                        if( !lua_isstring(L, -1) )
                            throw exception("[%s]: invalid actor #%u name, not a sting", ch, k);
                        lua_pop(L, 1);
                        ptr = lua_tolstring(L, -1, &len);
                        const string an(ptr,len);
                        
                    }
                    lua_pop(L,1);
                }
                
                
                //--------------------------------------------------------------
                //remove the equilibrium data
                //--------------------------------------------------------------
                lua_pop(L,1);
                
            }
            
        }
        
    }
    
}