#ifndef YOCTO_LUA_CONFIG_INCLUDED
#define YOCTO_LUA_CONFIG_INCLUDED 1

#include "yocto/lua/lua.hpp"
#include "yocto/string.hpp"

#include "yocto/container/sequence.hpp"

namespace yocto 
{
	
	namespace Lua
	{
		
		struct Config
		{
			//! implemented for lua_Number and string
			template <typename T>
			static T Get( lua_State *L, const string &name );
			
			//! implemented for lua_Number, bool and string
			template <typename T>
			static void GetTable( lua_State *L, const string &name, sequence<T> &seq );
			
			static void DoFile( lua_State *L, const string &filename );
            static void DoFile( lua_State *L, const char   *filename );
            
			static void DoString( lua_State *L, const string &string );
			
		};
		
	}
}

#endif
