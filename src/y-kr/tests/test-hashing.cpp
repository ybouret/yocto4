#include "yocto/utest/run.hpp"
#include "yocto/hashing/md5.hpp"

using namespace yocto;

YOCTO_UNIT_TEST_IMPL(hashing)
{
	hashing::md5 h_md5;

	hashing::function * h_reg[] = { &h_md5 };
	const size_t        h_num   = sizeof(h_reg)/sizeof(h_reg[0]);

	for( size_t i=0; i < h_num; ++i )
	{
		std::cerr << h_reg[i]->name() << std::endl;
	}

}
YOCTO_UNIT_TEST_DONE()
