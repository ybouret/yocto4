#include "yocto/utest/run.hpp"
#include "yocto/memory/slice.hpp"
#include "yocto/code/rand.hpp"

using namespace yocto;
using namespace memory;

YOCTO_UNIT_TEST_IMPL(slice)
{
	std::cerr << "sizeof(slice)=" << sizeof(slice) << std::endl;
	std::cerr << "sizeof(block_t)=" << sizeof(slice::block_t) << std::endl;
	{
		std::cerr << "block alignment" << std::endl;
		size_t s = 0;
		for( size_t i=0; i <10; ++i )
		{
			for( size_t j=0; j < 8; ++j, s += (i+j) )
			{
				std::cerr << " (" << s << ":" << slice::blocks_for(s)*sizeof(slice::block_t) << ")";
			}
			std::cerr << std::endl;
		}
	}

	{
		const size_t mini_size = 100 + alea_less_than<size_t>(2000);
		std::cerr << "#blocks for " << mini_size << " = " << slice::blocks_for( mini_size ) << std::endl;
		slice S(0,mini_size);
	}
}
YOCTO_UNIT_TEST_DONE()

