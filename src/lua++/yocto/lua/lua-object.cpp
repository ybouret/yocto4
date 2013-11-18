#include "yocto/lua/lua-object.hpp"
#include "yocto/lua/lua.hpp"

namespace yocto {

	namespace Lua {

		Object:: Object() throw() {
		}

		Object:: ~Object() throw() {

		}

		void Object:: SetClassName( lua_State *L ) const {
			luaL_getmetatable(L, this->GetClassName() );
			lua_setmetatable(L,-2);
		}

		int Object:: __rawget( lua_State *L ) {
			return luaL_error(L,"%s::__rawget(%f):not implemented",this->GetClassName(), luaL_checknumber(L,1));
		}

		int Object:: __rawset( lua_State *L ) {
			return luaL_error(L,"%s::__rawset(%f)='%s':not implemented",this->GetClassName(),luaL_checknumber(L,1),luaL_typename(L,2));
		}

	}

}
