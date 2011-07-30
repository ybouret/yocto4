#include "yocto/lua/lua-link.hpp"
#include "yocto/lua/lua.hpp"

namespace yocto {

	namespace Lua {

		//-- ints
		template <>
		int LinkAs<int>::Set( lua_State *L, void *v ) {\
			*(int *)v = luaL_checkint(L,3);
			return 0;
		}

		template <>
		int LinkAs<int>::Get( lua_State *L, void *v ) {
			lua_pushinteger( L, *(int *)v);
			return 1;
		}


		//-- lua_Number
		template <>
		int LinkAs<lua_Number>::Set( lua_State *L, void *v ) {
			*(lua_Number *)v = luaL_checknumber(L,3);
			return 0;
		}

		template <>
		int LinkAs<lua_Number>:: Get( lua_State *L, void *v ) {
			lua_pushnumber( L, *(lua_Number *)v);
			return 1;
		}

		//-- syquo string
		template <>
		int LinkAs<string>::Get( lua_State *L, void *v ) {
			const string &s = *(const string *)v;
			lua_pushlstring( L, &s[0], s.size() );
			return 1;
		}

		template <>
		int LinkAs<string>::Set( lua_State *L, void *v ) {
			string &s = *(string *)v;
			size_t len = 0;
			const char *txt = luaL_checklstring( L, 3, &len );
			s.assign( txt, len );
			return 0;
		}

		void Link::Expose( lua_State *L, const Link::Member *M ) {
			assert(L);
			assert(M);
			while(M->name) {
				lua_pushstring(L, M->name);          //! [g|s]etter name
				lua_pushlightuserdata(L, (void*)M ); //! [g|s]etter signature
				lua_rawset(L, -3);
				++M;
			}
		}

		int Link::Call(lua_State *L)
		{
			/* for get: stack has userdata, index, lightuserdata */
			/* for set: stack has userdata, index, value, lightuserdata */
			Link::Member *M = (Link::Member *)lua_touserdata(L, -1);  /* member info */
			lua_pop(L, 1);                                            /* drop lightuserdata */
			luaL_checktype(L, 1, LUA_TUSERDATA);
			char  **ppObj = (char **)lua_touserdata(L, 1);
			char  *ud = *ppObj;
			return M->access(L,  &ud[ M->offset ] );
		}



	}

}
