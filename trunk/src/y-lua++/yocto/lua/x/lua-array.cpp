#include "yocto/lua/x/lua-array.hpp"

namespace yocto {
	
	namespace Lua 
	{
		
		array:: ~array() throw()
		{
		}
		
		array:: array( lua_State *L ) : 
			array_(luaL_optint(L,1,0),0)
		{
			
		}
		
		
		int array:: size( lua_State *L)
		{
			lua_pushnumber( L, array_.size() );
			return 1;
		}
		
		int array:: capacity( lua_State *L )
		{
			lua_pushnumber( L, array_.capacity() );
			return 1;
		}
		
		int array:: __tostring( lua_State *L ) 
		{
			const array *a = Lua::Cast<array>(L,1);
			lua_pushliteral(L,"[");
			const ARRAY &A = a->array_;
			for( size_t i=1; i <= A.size(); ++i ) {
					lua_pushfstring( L, " %f", A[i] );
					lua_concat(L,2);
			}
			lua_pushliteral(L," ]'");
			lua_concat(L,2);
			return 1;
        }

		
	}
	
}
