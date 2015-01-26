#include "yocto/utest/run.hpp"
#include "yocto/rtld/module.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(module)
{
	if( argc > 1 )
	{
		dylib *dll = dylib_load( argv[1], dylib_full );
		const module M( dll );
		//dylib *dll2 = dylib_load( argv[1], dylib_lazy );
        //const module M2( dll2 );
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
