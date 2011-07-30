#include "yocto/lua/x/lua-vec3.hpp"

namespace yocto {

	namespace Lua {


		//-- implementation
		vec3:: vec3( lua_State *L ) :
				vtx()
		{
			const int narg = lua_gettop(L);
			switch( narg ) {
				case 0:
					break;

				case 1:
					vtx = Lua::Cast<vec3>(L,1)->vtx;
					break;


				case 2:
					vtx = Lua::Cast<vec3>(L,2)->vtx - Lua::Cast<vec3>(L,1)->vtx;
					break;

				case 3:
					vtx.x = luaL_checknumber(L,1);
					vtx.y = luaL_checknumber(L,2);
					vtx.z = luaL_checknumber(L,3);
					break;

				default:
					luaL_error(L,"vec3(wrong #args=%d)",narg);
			};

		}


		vec3:: vec3( const VEC3 &v ) throw() :
				vtx( v )
		{
		}

		vec3:: ~vec3() throw() {
		}

		int vec3:: length( lua_State *L ) {
			lua_pushnumber( L, vtx.norm() );
			return 1;
		}

		int vec3:: size( lua_State *L ) {
			lua_pushnumber( L, 3 );
			return 1;
		}


		int vec3:: __rawget( lua_State *L ) {

			switch( luaL_checkint(L,1) ) {
				case 1:
					lua_pushnumber(L, vtx.x );
					break;

				case 2:
					lua_pushnumber(L, vtx.y );
					break;

				case 3:
					lua_pushnumber(L, vtx.z );
					break;

				default:
					luaL_argerror(L,1,"vec3[index must be 1,2,3]");
					break;

			}
			return 1;
		}

		int vec3:: __rawset( lua_State *L ) {
			const lua_Number p = luaL_checknumber(L,2);
			switch( luaL_checkint(L,1) ) {
				case 1:
					vtx.x = p;
					break;

				case 2:
					vtx.y = p;
					break;

				case 3:
					vtx.z = p;
					break;

				default:
					luaL_argerror(L,1,"vec3[index must be 1,2,3]");
					break;
			}
			return 0;
		}


		int vec3:: version( lua_State *L ) {
			lua_pushliteral( L, __DATE__ );
			return 1;
		}


		int vec3::__tostring( lua_State *L ) {
			const vec3 *v = Lua::Cast<vec3>(L,-1);
			const VEC3 &r = v->vtx;
			lua_pushfstring(L,"[%f %f %f]'", r.x, r.y, r.z);
			return 1;
		}


		int vec3:: __add( lua_State *L ) {
			const vec3 *lhs = Lua::Cast<vec3>(L,1);
			const vec3 *rhs = Lua::Cast<vec3>(L,2);
			const VEC3  v = lhs->vtx + rhs->vtx;
			Lua::New<vec3,VEC3>(L,v);
			return 1;
		}

		int vec3:: __sub( lua_State *L ) {
			const vec3 *lhs = Lua::Cast<vec3>(L,1);
			const vec3 *rhs = Lua::Cast<vec3>(L,2);
			const VEC3 v = lhs->vtx - rhs->vtx;
			Lua::New<vec3,VEC3>(L,v);
			return 1;
		}

		int vec3:: __unm( lua_State *L ) {
			const vec3 *lhs = Lua::Cast<vec3>(L,1);
			const VEC3 v = - lhs->vtx;
			Lua::New<vec3,VEC3>(L,v);
			return 1;
		}

		int vec3:: __pow( lua_State *L ) {
			const vec3 *lhs = Lua::Cast<vec3>(L,1);
			const vec3 *rhs = Lua::Cast<vec3>(L,2);
			const VEC3 v = lhs->vtx ^ rhs->vtx;
			Lua::New<vec3,VEC3>(L,v);
			return 1;
		}


		int vec3:: __mul( lua_State *L ) {
			if( lua_isnumber(L,1) ) {
				//scalar times rhs
				const lua_Number lhs = lua_tonumber(L,1);
				const vec3      *rhs = Lua::Cast<vec3>(L,2);
				const VEC3       v   = lhs * (rhs->vtx);
				Lua::New<vec3,VEC3>(L,v);
				return 1;
			} else {
				const vec3 *lhs = Lua::Cast<vec3>(L,1);
				const vec3 *rhs = Lua::Cast<vec3>(L,2);
				lua_pushnumber(L, (lhs->vtx) * (rhs->vtx) );
				return 1;
			}

		}

	}

}
