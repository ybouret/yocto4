#include "yocto/lua/lua-config.hpp"
#include "yocto/exception.hpp"

namespace yocto 
{
	
	namespace Lua
	{
		
		static inline 
		void GetGlobal_( lua_State *L, const string &name )
		{
			assert( L );
			lua_settop( L, 0 );
			lua_getglobal(L, &name[0] );
		}
		
		template <>
		lua_Number Config::Get<lua_Number>( lua_State *L, const string &name )
		{
			GetGlobal_(L,name);
			if( !lua_isnumber(L,-1) )
				throw exception("'%s' is not a Number", &name[0] );
			return lua_tonumber(L,-1);
		}
        
        template <>
		bool Config::Get<bool>( lua_State *L, const string &name )
		{
			GetGlobal_(L,name);
			if( !lua_isboolean(L,-1) )
				throw exception("'%s' is not a Boolean", &name[0] );
			return lua_toboolean(L,-1) == 1;
		}
		
		template <>
		string Config::Get<string>( lua_State *L, const string &name )
		{
			GetGlobal_(L,name);
			if( !lua_isstring(L,-1) )
				throw exception("'%s' is not a String", &name[0] );
			size_t      len = 0;
			const char *str = lua_tolstring( L, -1, &len );
			return string( str, len );
		}
		
		template <>
		void Config:: GetTable<lua_Number>( lua_State *L, const string &name, sequence<lua_Number> &seq )
		{
			seq.free();
			
			GetGlobal_(L,name);
			if( ! lua_istable(L,-1) )
				throw exception("'%s' is not a table", &name[0] );
			const size_t Ns = lua_rawlen(L,-1);
			
			for( size_t i=1; i <= Ns; ++i )
			{
				lua_rawgeti(L,-1,i);
				seq.push_back( lua_tonumber(L,-1) );
				lua_pop(L,1);
			}
		}
		
		template <>
		void Config:: GetTable<string>( lua_State *L, const string &name, sequence<string> &seq )
		{
			seq.free();
			
			GetGlobal_(L,name);
			if( ! lua_istable(L,-1) )
				throw exception("'%s' is not a table", &name[0] );
			const size_t Ns = lua_rawlen(L,-1);
			
			for( size_t i=1; i <= Ns; ++i )
			{
				lua_rawgeti(L,-1,i);
				seq.push_back( lua_tostring(L,-1) );
				lua_pop(L,1);
			}
		}
		
		
		
		lua_State *Config:: DoFile( lua_State *L, const string &filename )
		{
			assert(L);
			lua_settop(L,0);
			if( luaL_dofile( L, &filename[0] ) )
			{
				throw exception("luaL_dofile( %s )", lua_tostring( L, -1 ) ); 
			}
            return L;
		}
        
        lua_State * Config:: DoFile( lua_State *L, const char *fn )
		{
            const string filename = fn;
            return Config::DoFile(L,filename);
        }
		
		void Config:: DoString( lua_State *L, const string &str  )
		{
			assert( L );
			lua_settop(L,0);
			if( luaL_dostring( L, str.c_str() ) )
			{
				throw exception("luaL_dostring( %s )", lua_tostring( L, -1 ) ); 
			}
		}
        
        void Config:: DoString( lua_State *L, const char *str  )
		{
			assert( L );
			lua_settop(L,0);
			if( luaL_dostring( L, str ) )
			{
				throw exception("luaL_dostring( %s )", lua_tostring( L, -1 ) );
			}
		}
	}
	
}
