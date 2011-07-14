#include "yocto/utest/run.hpp"
#include "yocto/rtld/module.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(module)
{
	if( argc > 1 )
	{
		module M( argv[1] );
		for( int i=2; i < argc; ++i )
		{
			std::cerr << argv[i] << ": ";
			void *addr = M.query( argv[i] );
			if( addr ) std::cerr << "@" << addr;
			else std::cerr << "NOT FOUND!";
			std::cerr << std::endl;
		}
	}
}
YOCTO_UNIT_TEST_DONE()
