#include "yocto/utest/run.hpp"
#include "yocto/sequence/c-array.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(c_array)
{
	int tab[] = { 1, 2, 4, 5 };
	c_array<int> arr( tab, sizeof(tab)/sizeof(tab[0]));
	std::cerr << "arr=" << arr << std::endl;
}
YOCTO_UNIT_TEST_DONE()
