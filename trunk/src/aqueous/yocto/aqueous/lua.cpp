#include "yocto/aqueous/lua.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/lua/lua-maths.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
	namespace aqueous
	{
        
		/////////////////////////////////////////////////////////////////////////////
		//
		// load a library = table of species
		//
		/////////////////////////////////////////////////////////////////////////////
        void _lua:: load( lua_State *L, library &lib, const char *libname, species_ctor *cb )
        {
            const string ln = libname;
            _lua::load(L,lib,ln,cb);
        }
        
		void _lua:: load( lua_State *L, library &lib, const string &libname, species_ctor *cb )
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
			const unsigned ns = lua_rawlen(L, -1);
			std::cerr << "*** Parsing " << ns << " species" << std::endl;
			for( unsigned i=1; i <= ns; ++i )
			{
                
				lua_rawgeti(L,-1,i); //------ get the species data -------------
                
                
				if( ! lua_istable(L, -1) )
					throw exception("species #%u: not a table", i);
                
				//-- get the number of fields
				const unsigned nf = lua_rawlen(L, -1);
				if( nf != 2 && nf != 3 )
					throw exception("species #%u: invalid #fields=%u, must be 2 or 3",i,nf);
                
                
				lua_rawgeti(L, -1, 1);  //-------- get the id ------------------
				if( !lua_isstring(L, -1) )
					throw exception("species #%u: first field is not a string",i);
				size_t       len = 0;
				const char  *ptr = lua_tolstring(L, -1, &len);
				const string id(ptr,len);
				const char  *ch = id.c_str();
				lua_pop(L,1);           //-------- pop the id ------------------
                
                
				lua_rawgeti(L, -1, 2);  //-------- get the algebraic charge ----
				if( !lua_isnumber(L, -1) )
					throw exception("%s: second field is not a number",ch);
				const int z = int( lua_tonumber(L, -1) );
				std::cerr << "***\t<" << id << ">, z=" << z << std::endl;
				lua_pop(L,1);           //-------- pop the algebraic charge ----
                
				//-- create the species
				species &sp = lib.add(id, z);
                
				if( (0 != cb)  )
				{
                    if( 3 == nf )
                    {
                        lua_rawgeti(L, -1, 3); //-------- get the extra field --
                        (*cb)(sp,L);           // call the ctor
                        lua_pop(L,1);          //-------- pop the extra field --
                    }
                    else
                    {
                        lua_pushnil(L);
                        (*cb)(sp,L);
                        lua_pop(L,1);
                    }
				}
                
				lua_pop(L,1);        //------ pop the species data -------------
			}
		}
        
		/////////////////////////////////////////////////////////////////////////////
		//
		// load a chemsys = table of equilibria
		//
		/////////////////////////////////////////////////////////////////////////////
        void _lua:: load(lua_State *L, yocto::aqueous::chemsys &cs, const char *csname)
        {
            const string csn = csname;
            _lua::load(L,cs,csn);
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
			const unsigned neq = lua_rawlen(L, -1);
			std::cerr << "*** Parsing " << neq << " equilibria" << std::endl;
			for( unsigned i=1; i <= neq; ++i )
			{
				lua_rawgeti(L,-1,i); //-------- get the equilibrum data --------
                
				if( !lua_istable(L, -1) )
					throw exception( "equilibrium #%u: not a table", i);
                
				const size_t nf = lua_rawlen(L, -1);
				if( nf < 3 )
					throw exception("equilibrium #%u: not enough fields",i);
                
                
				lua_rawgeti(L,-1,1); //-------- get the name -------------------
				if( !lua_isstring(L, -1) )
					throw exception("equilibrium #%u: first field is not a string", i);
				size_t       len = 0;
				const char  *ptr = lua_tolstring(L, -1, &len);
				const string id(ptr,len);
				const char  *ch = id.c_str();
				lua_pop(L,1);        //-------- pop the name -------------------
                
                
				equilibrium *eq = NULL;
				lua_rawgeti(L, -1,2); //-------- get the constant --------------
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
				lua_pop(L,1);         //-------- pop the constant --------------
                
				assert( NULL != eq );
                
				//--------------------------------------------------------------
				//-- parse the actors
				//--------------------------------------------------------------
				for( unsigned j=3,k=1; j <= nf; ++j, ++k )
				{
                    
					lua_rawgeti(L,-1,j); //-------- get the actor table --------
					{
						if( !lua_istable(L, -1) )
							throw exception("[%s]: invalid actor #%u", ch, k );
						if( lua_rawlen(L, -1) != 2 )
							throw exception("[%s]: invalid actor #%u format", ch, k );
                        
                        
						lua_rawgeti(L, -1, 2); //-------- get the actor name ---
						if( !lua_isstring(L, -1) )
							throw exception("[%s]: invalid actor #%u name, not a string", ch, k);
						ptr = lua_tolstring(L, -1, &len);
						const string ac(ptr,len);
						const char  *acn = ac.c_str();
						lua_pop(L, 1);         //-------- pop the actor name ---
                        
                        
						lua_rawgeti(L, -1, 1); //-- get the actor coefficient --
						if( !lua_isnumber(L, -1) )
							throw exception("[%s]: invalid {%s} coefficient",ch,acn);
						const int nu = int( lua_tonumber(L, -1) );
						lua_pop(L, 1);         //-- pop the actor coefficient --
						if( 0 == nu )
							throw exception("[%s]: zero coefficient for {%s}",ch,acn);
                        
						// create the actor
						eq->add( ac, nu );
					}
					lua_pop(L,1);        //-------- pop the actor table --------
				}
                
				std::cerr << "***\t\t" << *eq << std::endl;
                
				lua_pop(L,1);        //-------- pop the equilibrum data --------
                
			}
            
		}
        
        
		///////////////////////////////////////////////////////////////////////
		//
		// load an initializer = table of constraints
		//
		///////////////////////////////////////////////////////////////////////
        void _lua:: load( lua_State *L, initializer &ini, const char *ininame )
        {
            const string inn = ininame;
            _lua::load(L,ini,inn);
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
			// parse all the constraints
			//==================================================================
			const unsigned nc = lua_rawlen(L, -1);
			std::cerr << "*** Parsing " << nc << " constraints" << std::endl;
			for( unsigned i=1; i <= nc; ++i )
			{
				lua_rawgeti(L, -1, i); //-------- get  the constraint ----------
				if( !lua_istable(L, -1) )
					throw exception("%s: invalid constraint #%u", ini_name, i);
                
				const size_t nf = lua_rawlen(L, -1);
				if( nf < 2 )
					throw exception("%s: not enough data for constraint #%u", ini_name, i);
                
				constraint *pC = NULL;
				lua_rawgeti(L, -1, 1); //-------- get the constraint value -----
				if( lua_isnumber(L,-1) )
				{	
					// fixed constraint
					pC = & ini.create( lua_tonumber(L,-1 ) );
				}
				else
				{
					// name of the function
					if( lua_isstring(L, -1) )
					{
						size_t      len = 0;
						const char *ptr = lua_tolstring(L, -1, &len);
						const string fn(ptr,len);
						Lua::Function<double>       kfn(L,fn); // functionoid
						const initproc              v( kfn );  // functor
						pC = &ini.create( v );
                        (bool&)(ini.is_variable)  = true;
					}
					else 
						throw exception("%s: invalid type for constraint #%u value", ini_name, i);
				}
				assert(NULL!=pC);
				lua_pop(L, 1);         //-------- pop the constraint value -----
                
                std::cerr << "***\t" << pC->value(0.0) << "=";
				for( unsigned j=2,k=1; j <= nf; ++j, ++k )
				{
                    
					lua_rawgeti(L,-1,j); //-------- get the item table ---------
					{
						if( !lua_istable(L, -1) )
							throw exception("%s: constraint #%u has invalid field #%u", ini_name, i, k );
						if( lua_rawlen(L, -1) != 2 )
							throw exception("%s: constraint #%u has invalid field #%u format", ini_name, i, k );
                        
                        
						lua_rawgeti(L, -1, 2); //-------- get the item name ----
						if( !lua_isstring(L, -1) )
							throw exception("%s: constraint #%u, field #%u: invalid name", ini_name, i, k);
						size_t       len = 0;
						const char  *ptr = lua_tolstring(L, -1, &len);
						const string id(ptr,len);
						const char  *sp = id.c_str();
						lua_pop(L, 1);         //-------- pop the item name ----
                        
                        
						lua_rawgeti(L, -1, 1); //-------- get the coefficient --
						if( !lua_isnumber(L, -1) )
							throw exception("%s: constraint #%u, field #%u: invalid {%s} coefficient",ini_name,i,k,sp);
						const double w = lua_tonumber(L, -1);
						lua_pop(L, 1);         //-------- pop the coefficient --
                        
						// create the constraint
						pC->add( id, w);
                        std::cerr << "+(" << w << ")[" << id << "]";
					}
					lua_pop(L,1);        //-------- pop the item table ---------
				}
                std::cerr << std::endl;

				lua_pop(L,1);          //-------- get  the constraint ----------
			}
		}
        
        
        
	}
    
}
