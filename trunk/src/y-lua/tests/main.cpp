#include "yocto/lua/lua.hpp"
#include "yocto/exceptions.hpp"

#include <iostream>
#include <cerrno>

using namespace yocto;

int main( int argc, char *argv[] )
{
	lua_State *L = NULL;
	try 
	{
		L = luaL_newstate();
		if( !L )
			throw libc::exception( ENOMEM, "lua_open");
		luaL_openlibs(L);
		
		for( int i=1; i < argc; ++i )
		{
			const char *filename = argv[i];
			if( luaL_dofile( L, filename ) )
			{
				throw imported::exception( lua_tostring(L,-1), "lua_dofile(%s)", filename );
			}
		}
		
		lua_close(L);
	}
	catch( const exception &e )
	{
		std::cerr << e.what() << std::endl;
		std::cerr << e.when() << std::endl;
		if(L)
		lua_close(L);
		return -1;
	}
	return 0;
}
