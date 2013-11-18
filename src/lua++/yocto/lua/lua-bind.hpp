/** \file  lua_bind.hpp
    \brief C++ <-> Lua code binding
*/

#ifndef YOCTO_LUA_BIND_INCLUDED
#define YOCTO_LUA_BIND_INCLUDED 1

#include "yocto/lua/lua-build.hpp"

#include <iostream>

namespace yocto {

	namespace Lua {

		//! templated class binding
		template <class OBJECT>
		class Bind {
			public:
				typedef typename OBJECT::Method Method;

				//! multiple steps registration
				static
				inline void Register( lua_State *L ) {

					//--------------------------------------------------------------
					//
					// constructor
					//
					//--------------------------------------------------------------
					lua_register( L, OBJECT::ClassName, Lua::New<OBJECT> );

					//--------------------------------------------------------------
					//
					// C++ methods
					//
					//--------------------------------------------------------------
					lua_newtable(L);                                 //! methods table
					const int methods = lua_gettop(L);               //! mark methods.


					//! append C++ wrappers
					for( const Method *M = OBJECT::Methods; M->Name != NULL; ++M ) {
						std::cout << "register [" << M->Name << "]" << std::endl;
						lua_pushstring(L, M->Name);
						lua_pushlightuserdata(L, (void*)M );
						lua_pushcclosure(L, Bind<OBJECT>::Dispatch, 1);
						lua_rawset(L, methods);
					}

					//--------------------------------------------------------------
					//
					// Package Functions
					//
					//--------------------------------------------------------------
					static const luaL_Reg _defaultFunctions[] =
					    {
					        { "new", Lua::New<OBJECT> },
					        { NULL, NULL }
					    };

					lua_newtable(L);
					luaL_setfuncs(L, _defaultFunctions,0 );
					luaL_setfuncs(L,   OBJECT::Functions,0 );
					lua_setglobal(L, OBJECT::PackageName     );


					//--------------------------------------------------------------
					//
					// Metatable = Events
					//
					//--------------------------------------------------------------
					static const luaL_Reg _defaultEvents[] =
					    {
					        { "__gc", Lua::Delete<OBJECT> },
					        { NULL, NULL }
					    };

					luaL_newmetatable(L, OBJECT::ClassName );  //! metatable
					luaL_setfuncs(L, _defaultEvents, 0 );      //! default metamethods
					luaL_setfuncs(L, OBJECT::Events, 0 );      //! user's behavior
					const int metatable = lua_gettop(L);       //! mark metatable.


					lua_pushliteral(L, "__metatable");
					lua_pushvalue(L, methods);  //! dup methods table
					lua_rawset(L, metatable);   //! hide metatable: metatable.__metatable = methods


					//----------------------------------------------------------
					//
					// building __index
					//
					//----------------------------------------------------------
					lua_pushliteral(L, "__index");
					lua_pushvalue(L, metatable);                  /* upvalue index 1 */
					Link::Expose(L, OBJECT::Getters );            /* fill metatable with getters */
					lua_pushvalue(L, methods);                    /* upvalue index 2 */
					lua_pushcclosure(L, Bind<OBJECT>::OnGet, 0 ); /* special OnGet: upvalue 3 */
					lua_pushcclosure(L, __index_bind, 3);
					lua_rawset(L, metatable);             /* metatable.__index = this::__index */

					//----------------------------------------------------------
					//
					// building __new_index
					//
					//----------------------------------------------------------
					lua_pushliteral(L, "__newindex");
					lua_newtable(L);                              /* table for members you can set */
					Link::Expose(L, OBJECT::Setters );            /* fill with setters:   upvalue 1*/
					lua_pushcclosure(L, Bind<OBJECT>::OnSet, 0 ); /* special OnSet value: upvalue 2*/
					lua_pushcclosure(L, __newindex_bind, 2);
					lua_rawset(L, metatable);                 /* metatable.__newindex = this::__newindex */
					lua_pop(L, 2);                            /* drop metatable/methods */

				}



			private:
				//! dispatch named method
				static int Dispatch( lua_State *L ) {
					//--------------------------------------------------------------
					// stack has userdata, followed by method args
					//--------------------------------------------------------------
					OBJECT *pObj = Lua::Cast<OBJECT>(L, 1);  //! get 'self', or if you prefer, 'this'
					lua_remove(L, 1);                       //! remove self so member function args start at index 1

					//--------------------------------------------------------------
					// get member function from upvalue
					//--------------------------------------------------------------
					Method *M = static_cast<Method *>( lua_touserdata(L, lua_upvalueindex(1)) );
					return (pObj->*(M->Addr))(L);  //! call member function
				}

				//! special get OBJECT[Number]
				static int OnGet( lua_State *L ) {
					//----------------------------------------------------------
					// stack has userdata, followed by a number
					//----------------------------------------------------------
					Lua::Object *pObj = Lua::Cast<OBJECT>(L,1); //! get 'this'
					lua_remove(L,1);
					assert( lua_isnumber(L,1) );
					return pObj->__rawget( L );
				}

				//! special set OBJECT[Number]
				static int OnSet( lua_State *L ) {
					//----------------------------------------------------------
					// stack has userdata, followed by a number and a value
					//----------------------------------------------------------
					Lua::Object *pObj = Lua::Cast<OBJECT>(L,1); //! get 'this'
					lua_remove(L,1);
					assert(  lua_isnumber(L,1) );
					assert( !lua_isnil(L,2) );
					return pObj->__rawset( L );
				}

				YOCTO_DISABLE_COPY_AND_ASSIGN(Bind);
				Bind();
				~Bind();
		};

		template <class OBJECT>
		inline void Require( lua_State *L )
		{
			Lua::Bind<OBJECT>::Register(L);
		}

	}
}

#endif
