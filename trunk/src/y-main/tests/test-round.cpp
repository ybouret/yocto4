#include "yocto/utest/run.hpp"
#include "yocto/code/round.hpp"
#include "yocto/threading/mutex.hpp"

using namespace yocto;
using namespace threading;

YOCTO_UNIT_TEST_IMPL(round)
{
	for( size_t i=0; i < 10; ++i )
	{
		std::cerr << i;
		for( size_t j=0; j < 8; ++j )
		{
			std::cerr << " " << YOCTO_ROUND(j,i);
		}
		std::cerr << std::endl;
	}
	std::cerr << "sizeof(mutex)=" << sizeof(mutex) << std::endl;
	std::cerr << "#u64         =" << YOCTO_U64_FOR_ITEM(mutex) << std::endl;
	uint64_t dummy[ YOCTO_U64_FOR_ITEM(mutex) ];
	std::cerr << "sizeof(dummy)=" << sizeof(dummy) << std::endl;

}
YOCTO_UNIT_TEST_DONE()
