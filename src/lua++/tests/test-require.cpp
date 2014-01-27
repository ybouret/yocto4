#include "yocto/utest/run.hpp"

#include "yocto/lua/lua-state.hpp"
#include "yocto/lua/lua-config.hpp"

#include "yocto/lua/x/lua-array.hpp"
#include "yocto/lua/x/lua-vec3.hpp"

#include "yocto/lua/lua-bind.hpp"

#include "yocto/exceptions.hpp"

#include <iostream>
#include <cerrno>

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(require)
{
	
	
	try
	{
		Lua::State vm;
		lua_State *L = vm();
		Lua::Require< Lua::array >( L );
		Lua::Require< Lua::vec3  >( L );
        
		for( int i=1; i < argc; ++i )
		{
			const char *filename = argv[i];
			Lua::Config::DoFile( L, filename );
		}
		
	}
	catch( const exception &e )
	{
		std::cerr << e.what() << std::endl;
		std::cerr << e.when() << std::endl;
		return -1;
    }
}
YOCTO_UNIT_TEST_DONE()
