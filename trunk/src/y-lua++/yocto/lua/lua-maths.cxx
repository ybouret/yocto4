#include "yocto/lua/lua-maths.hpp"
#include "yocto/lua/lua.hpp"
#include "yocto/math/ztype.hpp"
#include "yocto/exception.hpp"

namespace yocto
{
	
	
	namespace Lua
	{
		using math::real_t;
		
		template<>
		real_t Function<real_t>:: operator()( real_t x )
		{
			
			lua_getglobal( L, &name[0] );
			assert( lua_isfunction(L,-1) );
			lua_pushnumber(L,x); //-- push x onto the stack
			if( lua_pcall(L,1,1,0) )
				throw exception( "%s", lua_tostring(L,-1) );
			const double ans = lua_tonumber(L,-1);
			lua_pop(L,1);  //-- the result
			return real_t(ans);
		}
		
		template <>
		Function<real_t>:: Function( lua_State *s, const string &id, bool check ) :
		L( s ),
		name( id )
		{
			assert( L != NULL );
			if( check )
			{
				lua_getglobal( L, &name[0] );
				if( !lua_isfunction(L,-1) )
					throw exception("%s is not a function !", &id[0] );
				lua_pop(L,1);
			}
		}
		
		
		
		
		template <>
		Function<real_t>:: ~Function() throw()
		{
			
		}
		
		template <>
		Function<real_t>:: Function( const Function &F ) :
		L( F.L ),
		name( F.name )
		{
		}
		
		template <>
		Function<real_t> & Function<real_t>:: operator=( const Function &F ) 
		{
			string &rname = *(string *) &name;
			rname.assign( F.name );
			L = F.L;
			return *this;
		}
		
		
		
		template<>
		real_t Function<real_t>:: operator()( real_t x, real_t y )
		{
			
			lua_getglobal( L, &name[0] );
			assert( lua_isfunction(L,-1) );
			lua_pushnumber(L,x); //-- push x onto the stack
			lua_pushnumber(L,y); //-- push y onto the stack
			if( lua_pcall(L,2,1,0) )
				throw exception( "%s", lua_tostring(L,-1) );
			const double ans = lua_tonumber(L,-1);
			lua_pop(L,1);  //-- the result
			return real_t(ans);
		}
		
	}
	
	
}
