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
                
                //-- get the constant: function or constant
                equilibrium *eq = NULL;
                lua_rawgeti(L, -1,2);
                if( lua_isnumber(L,-1) )
                {
                    // create with a real constant
                    const double K = lua_tonumber(L, -1);
                    eq = & cs.create(id, K);
                }
                else 
                {
                    // create with a function name
                    if( lua_isstring(L, -1) )
                    {
                        ptr = lua_tolstring(L, -1, &len);
                        const string fn(ptr,len);
                        Lua::Function<double>       kfn(L,fn); // functionoid
                        const equilibrium::constant K( kfn );  // functor
                        eq = & cs.create(id, K);
                    }
                    else 
                        throw exception("[%s]: invalid constant type", ch);
                }
                lua_pop(L,1);
                
                assert( NULL != eq );
                
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
                            throw exception("[%s]: invalid actor #%u name, not a string", ch, k);
                        ptr = lua_tolstring(L, -1, &len);
                        const string ac(ptr,len);
                        const char  *acn = ac.c_str();
                        lua_pop(L, 1); // remove the actor name
                        
                        // get the actor coefficient
                        lua_rawgeti(L, -1, 1);
                        if( !lua_isnumber(L, -1) )
                            throw exception("[%s]: invalid {%s} coefficient",ch,acn);
                        const int nu = int( lua_tonumber(L, -1) );
                        lua_pop(L, 1); // remove the actor coefficient
                        if( 0 == nu )
                            throw exception("[%s]: zero coefficient for {%s}",ch,acn);
                        
                        // Ok, create the actor
                        eq->add( ac, nu );
                    }
                    lua_pop(L,1); // remove the actor table
                }
                
                std::cerr << "***\t\t" << *eq << std::endl;
                //--------------------------------------------------------------
                //remove the equilibrium data
                //--------------------------------------------------------------
                lua_pop(L,1);
                
            }
            
        }
        
        void _lua:: load( lua_State *L, initializer &ini, const string &ininame )
        {
            assert(L!=NULL);
            //==================================================================
            // load ini table
            //==================================================================
            const char *ini_name = ininame.c_str();
            lua_settop(L, 0);
            lua_getglobal(L, ini_name);
            if( !lua_istable(L, -1) )
                throw exception("%s is not a table", ini_name );
            
            //==================================================================
            // load the chemsys table
            //==================================================================
            const unsigned nc = lua_objlen(L, -1);
            std::cerr << "*** Parsing " << nc << " constraints" << std::endl;
            for( unsigned i=1; i <= nc; ++i )
            {
                lua_rawgeti(L, -1, i); // -------- load the constraint
                if( !lua_istable(L, -1) )
                    throw exception("%s: invalid constraint #%u", ini_name, i);
                
                const size_t nf = lua_objlen(L, -1);
                if( nf < 2 )
                    throw exception("%s: not enough data for constraint #%u", ini_name, i);
                
                lua_rawgeti(L, -1, 1); // ------------ load constraint value
                if( !lua_isnumber(L, -1) )
                    throw exception("%s: invalid type for constraint #%u value", ini_name, i);
                constraint &c = ini.create( lua_tonumber(L, -1));
                lua_pop(L, 1);         // ------------ remove contraint value
                
                for( unsigned j=2,k=1; j <= nf; ++j, ++k )
                {
                    
                    lua_rawgeti(L,-1,j); // -------- get the item table
                    {
                        if( !lua_istable(L, -1) )
                            throw exception("%s: constraint #%u has invalid field #%u", ini_name, i, k );
                        if( lua_objlen(L, -1) != 2 )
                            throw exception("%s: constraint #%u has invalid field #%u format", ini_name, i, k );
                        
                        
                        lua_rawgeti(L, -1, 2); // -------- get the item name
                        if( !lua_isstring(L, -1) )
                            throw exception("%s: constraint #%u, field #%u: invalid name", ini_name, i, k);
                        size_t       len = 0;
                        const char  *ptr = lua_tolstring(L, -1, &len);
                        const string id(ptr,len);
                        const char  *sp = id.c_str();
                        lua_pop(L, 1); // remove the actor name
                        
                        
                        lua_rawgeti(L, -1, 1); // -------- get the item coefficient
                        if( !lua_isnumber(L, -1) )
                            throw exception("%s: constraint #%u, field #%u: invalid {%s} coefficient",ini_name,i,k,sp);
                        const double w = lua_tonumber(L, -1);
                        lua_pop(L, 1);        // -------- remove the item coefficient
                        
                        // create the constraint
                        c.add( id, w);
                    }
                    lua_pop(L,1);       // -------- remove the item  table

                }
                
                lua_pop(L,1);          // -------- remove the constraint
            }
        }
        
        
    }
    
}