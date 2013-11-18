#include "yocto/utest/run.hpp"
#include "yocto/container/container.hpp"

#include "support.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(container)
{
	size_t n = 0;
	for( size_t i=0; i < 16; ++i )
	{
		std::cerr << n << " -> ";
		n = container::next_capacity(n);
		std::cerr << n << std::endl;
	}
}
YOCTO_UNIT_TEST_DONE()
