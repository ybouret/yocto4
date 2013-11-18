#include "yocto/lua/lua-link.hpp"

namespace yocto {

	namespace Lua {

		int __index_bind( lua_State *L ) {

			/* stack has userdata, index */
			if( lua_isnumber(L,2) ) {
				lua_pushvalue(L, lua_upvalueindex(3) );   // get Bind<...>::OnGet
				assert( lua_iscfunction(L,-1) );
				lua_pushvalue(L,1);                      // userdata
				lua_pushvalue(L,2);                      // number
				lua_call(L,2,LUA_MULTRET);               // call this->rawget()
				assert(lua_gettop(L)>=2);
				return lua_gettop(L) - 2;
			}
			else {

				lua_pushvalue(L, 2);                     /* dup index */
				lua_rawget(L, lua_upvalueindex(1));      /* lookup member by name */

				if (!lua_islightuserdata(L, -1)) {
					lua_pop(L, 1);                         /* drop nil */
					lua_pushvalue(L, 2);                   /* dup index */
					lua_gettable(L, lua_upvalueindex(2));  /* else try methods */
					if (lua_isnil(L, -1))                  /* invalid member */
					{
						luaL_error(L, "cannot get member '%s'", lua_tostring(L, 2));
						return 0;
					} else {
						return 1; //-- method
					}
				}
				else {
					return Link::Call(L);                      /* call get function */
				}

			}
		}

		int __newindex_bind(lua_State *L)
		{


			if( lua_isnumber(L,2) ) {
				lua_pushvalue(L, lua_upvalueindex(2) );   // get Bind<...>::OnSet
				assert( lua_iscfunction(L,-1) );
				lua_pushvalue(L,1);                      // userdata
				lua_pushvalue(L,2);                      // number
				lua_pushvalue(L,3);                      // argument


				lua_call(L,3,LUA_MULTRET);               // call this->rawset()
				assert(lua_gettop(L)>=2);
				return lua_gettop(L) - 2;
			}
			else {
				/* stack has userdata, index, value */
				lua_pushvalue(L, 2);                     /* dup index */
				lua_rawget(L, lua_upvalueindex(1));      /* lookup member by name */
				if (!lua_islightuserdata(L, -1))         /* invalid member */
					luaL_error(L, "cannot set member '%s'", lua_tostring(L, 2));
				return Link::Call(L);                      /* call set function */
			}
		}


	}

}
